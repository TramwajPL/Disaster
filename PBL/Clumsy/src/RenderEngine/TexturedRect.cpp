
#include "../pch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "TexturedRect.h"

namespace Clumsy
{
	TexturedRect::TexturedRect(std::string path, glm::vec3 topRightCorner, glm::vec3 bottomRightCorner, glm::vec3 bottomLeftCorner, glm::vec3 topLeftCorner) : path(path)
	{
		BackgroundInit(topRightCorner, bottomRightCorner, bottomLeftCorner, topLeftCorner);
	}

	void TexturedRect::Render(Shader* shader)
	{
		if (m_Enabled)
		{
			glEnable(GL_TEXTURE_2D);

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, texture);

			glDisable(GL_CULL_FACE);
			// render container
			shader->use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glEnable(GL_CULL_FACE);


		}
	}

	void TexturedRect::BackgroundInit(glm::vec3 topRightCorner, glm::vec3 bottomRightCorner, glm::vec3 bottomLeftCorner, glm::vec3 topLeftCorner)
	{
		float vertices[] = {
			// positions						// colors           // texture coords
			topRightCorner.x, topRightCorner.y, topRightCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			bottomRightCorner.x, bottomRightCorner.y, bottomRightCorner.z,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			topLeftCorner.x, topLeftCorner.y, topLeftCorner.z,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);


		// load and create a texture 
		// -------------------------
		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		stbi_set_flip_vertically_on_load(false);

	}

}