#include "../pch.h"

#include "PostProcessor.h"

namespace Clumsy
{
    PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height) : 
        m_PostProcessingShader(shader), 
        m_Texture(), 
        m_Width(width), 
        m_Height(height), 
        m_Grey(GL_FALSE),
        m_Shake(GL_FALSE)
    {
        // Initialize renderbuffer/framebuffer object
        glGenFramebuffers(1, &this->DepthFBO);
        glGenFramebuffers(1, &this->MSFBO);
        glGenFramebuffers(1, &this->FBO);
        glGenRenderbuffers(1, &this->RBO);
        glGenRenderbuffers(1, &this->DepthRBO);

        // depth
        glBindFramebuffer(GL_FRAMEBUFFER, this->DepthFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, this->DepthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height); // Allocate storage for render buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->DepthRBO); // Attach MS render buffer object to framebuffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize DepthFBO" << std::endl;

        // Initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); // Allocate storage for render buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); // Attach MS render buffer object to framebuffer
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

        // Also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
        glGenTextures(1, &m_Texture.ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.ID, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, this->DepthFBO);
        glGenTextures(1, &m_Texture2.ID);
        glActiveTexture(GL_TEXTURE1); // activating the 2nd texture
        glBindTexture(GL_TEXTURE_2D, m_Texture2.ID);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_Texture2.ID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Initialize render data and uniforms
        this->initRenderData();
        this->m_PostProcessingShader.SetInteger("scene", 0, GL_TRUE);
        GLfloat offset = 1.0f / 300.0f;
        GLfloat offsets[9][2] = {
            { -offset,  offset  },  // top-left
            {  0.0f,    offset  },  // top-center
            {  offset,  offset  },  // top-right
            { -offset,  0.0f    },  // center-left
            {  0.0f,    0.0f    },  // center-center
            {  offset,  0.0f    },  // center - right
            { -offset, -offset  },  // bottom-left
            {  0.0f,   -offset  },  // bottom-center
            {  offset, -offset  }   // bottom-right    
        };
        glUniform2fv(glGetUniformLocation(this->m_PostProcessingShader.ID, "offsets"), 9, (GLfloat*)offsets);
        GLint edge_kernel[9] = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };
        glUniform1iv(glGetUniformLocation(this->m_PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
        GLfloat blur_kernel[9] = {
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
        };
        glUniform1fv(glGetUniformLocation(this->m_PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
    }

    void PostProcessor::BeginRender()
    {
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->DepthFBO);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    void PostProcessor::EndRender()
    {
        // Now resolve multisampled color-buffer into intermediate FBO to store to texture
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
        glBlitFramebuffer(0, 0, this->m_Width, this->m_Height, 0, 0, this->m_Width, this->m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Binds both READ and WRITE framebuffer to default framebuffer
    }

    void PostProcessor::Render(GLfloat time)
    {
        // Set uniforms/options
        this->m_PostProcessingShader.use();
        this->m_PostProcessingShader.setFloat("time", time);
        this->m_PostProcessingShader.setInt("grey", this->m_Grey);
        this->m_PostProcessingShader.setInt("shake", this->m_Shake);
        // Render textured quad
        glActiveTexture(GL_TEXTURE0);
        this->m_Texture.Bind();
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void PostProcessor::initRenderData()
    {
        // Configure VAO/VBO
        GLuint VBO;
        GLfloat vertices[] = {
            // Pos        // Tex
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}