#include "../pch.h"

#include "Cube.h"
#include "RenderEngine.h"
#include "../Components/RenderModelComponent.h"

namespace Clumsy 
{
    Cube::Cube(Transform transform) : GameObject(transform)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        float cubeVertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, // Bottom-left
             0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, // bottom-right   
             0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, // top-right             
             0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, // top-right
            -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, // top-left
            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, // bottom-left               
            // Front face
            -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // bottom-left
             0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top-right
             0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // bottom-right       
             0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top-right
            -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // bottom-left
            -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top-left       
            // Left face
            -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // top-right
            -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // bottom-left
            -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // top-left      
            -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // bottom-left
            -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // top-right
            -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // bottom-right
            // Right face
             0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // top-left
             0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // top-right     
             0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // bottom-right         
             0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // bottom-right
             0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // bottom-left
             0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // top-left
            // Bottom face      
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, // top-right
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, // bottom-left
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, // top-left       
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, // bottom-left
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, // top-right
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, // bottom-right
            // Top face
            -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, // top-left
             0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, // top-right
             0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, // bottom-right                
             0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, // bottom-right
            -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, // bottom-left
            -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f // top-left    
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    void Cube::Render(Shader* shaderCube)
    {
        if (m_Render)
        {
            shaderCube->use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Transform.GetPos());
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            model = glm::scale(model, m_Scale);
            shaderCube->setMat4("model", model);
            shaderCube->setMat4("view", RenderEngine::GetInstance()->getView());
            shaderCube->setMat4("projection", RenderEngine::GetInstance()->getProjection());

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

        }
	}

    void Cube::Update()
    {
        glm::vec3 parent = m_Player->m_Transform.GetPos();
        glm::vec3 newPos = parent + glm::vec3(0.0f, 1.5f, 0.0f);
        m_Transform.SetPos(newPos);

        if (TurnSystem::GetInstance()->GetActivePlayer()->m_Rmc == m_Player)
        {
            m_Render = true;
        }
        else
        {
            m_Render = false;
        }
    }

    void Cube::SetPlayer(RenderModelComponent* rmc)
    {
        m_Player = rmc;
    }
}


