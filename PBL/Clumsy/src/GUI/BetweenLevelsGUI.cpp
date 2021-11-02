
#include "../pch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "CreditsGUI.h"
#include "BetweenLevelsGUI.h"
#include "../RenderEngine/RenderEngine.h"

namespace Clumsy
{
	BetweenLevelsGUI::BetweenLevelsGUI()
	{
		BackgroundInit();

		//Buttons
		Button* b1 = new Button(glm::vec2(-0.01f, -0.5f), "NEXT LEVEL", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.7f, 0.1f));
		m_Buttons.push_back(b1);

		gui = new GUI();
	}

	void BetweenLevelsGUI::Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT)
	{
		if (m_Enabled)
		{
			glEnable(GL_TEXTURE_2D);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, texture);

			glDisable(GL_CULL_FACE);
			// render container
			shader->use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glEnable(GL_CULL_FACE);

			// Render buttons
			for (int i = 0; i < m_Buttons.size(); i++)
			{
				m_Buttons[i]->Render(shaderButton);
			}

			// Render text
			for (int i = 0; i < m_Buttons.size(); i++)
			{
				gui->RenderText(shaderText, m_Buttons[i]->GetText(), SCR_WIDTH / 2 - 110, SCR_HEIGHT - 825 - 80 * i, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			}

			gui->RenderText(shaderText, "Good Job! ", SCR_WIDTH / 2 - 200, SCR_HEIGHT - 200.0f, 1.3f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Statistics: ", SCR_WIDTH / 2 - 300, SCR_HEIGHT - 300.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Cut trees: " + std::to_string(RenderEngine::GetInstance()->GetCutTreesCounter()), SCR_WIDTH / 2 - 300, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Burnt trees: " + std::to_string(RenderEngine::GetInstance()->GetBurntTreesCounter()), SCR_WIDTH / 2 - 300, SCR_HEIGHT - 400.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Achievements: ", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 300.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			if ((RenderEngine::GetInstance()->GetCutTreesCounter() + RenderEngine::GetInstance()->GetBurntTreesCounter()) < 20)
			{
				gui->RenderText(shaderText, "Destroyer", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else if ((RenderEngine::GetInstance()->GetCutTreesCounter() + RenderEngine::GetInstance()->GetBurntTreesCounter()) > 20 && (RenderEngine::GetInstance()->GetCutTreesCounter() + RenderEngine::GetInstance()->GetBurntTreesCounter()) < 30)
			{
				gui->RenderText(shaderText, "Advanced destroyer", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			 else if ((RenderEngine::GetInstance()->GetCutTreesCounter() + RenderEngine::GetInstance()->GetBurntTreesCounter()) > 30)
			{
				gui->RenderText(shaderText, "Ultimate destroyer", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}

	void BetweenLevelsGUI::BackgroundInit()
	{
		float vertices[] = {
			// positions          // colors           // texture coords
			 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
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
		unsigned char* data = stbi_load(std::string("../Clumsy/src/models/between.jpg").c_str(), &width, &height, &nrChannels, 0);
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

	void BetweenLevelsGUI::HandleButtonClick(float screenX, float screenY, GLFWwindow* glfwWindow)
	{
		if (screenX > (m_Buttons[0]->GetCorner().x - (m_Buttons[0]->GetScale().x / 2)) && screenX < (m_Buttons[0]->GetCorner().x + (m_Buttons[0]->GetScale().x / 2))
			&& screenY < (m_Buttons[0]->GetCorner().y + m_Buttons[0]->GetScale().y) && screenY > m_Buttons[0]->GetCorner().y)
		{
			m_Buttons[0]->OnClick();
			m_Enabled = false;
		}

	}
}