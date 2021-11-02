#include "../pch.h"
#include <glad/glad.h>
#include <stb_image.h>
#include "EndGameGUI.h"
#include "../Core/CoreEngine.h"
#include "../RenderEngine/RenderEngine.h"
#include "../RenderEngine/Window.h"

namespace Clumsy {

	EndGameGUI::EndGameGUI()
	{
		BackgroundInit();
		Button* play = new Button(glm::vec2(-0.01f, -0.10f), "PLAY AGAIN", glm::vec3(0.f, 0.0f, 0.0f), glm::vec2(0.35f, 0.12f));
		m_Buttons.push_back(play);
		Button* quit = new Button(glm::vec2(-0.01f, -0.30f), "QUIT", glm::vec3(0.f, 0.0f, 0.0f), glm::vec2(0.2f, 0.12f));
		m_Buttons.push_back(quit);

		//theEndText = new GUI();
		//quitButton = new GUI();
		//playAginButton = new GUI();
		gui = new GUI();
	}

	EndGameGUI::~EndGameGUI()
	{
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void EndGameGUI::Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT)
	{
		if (m_Enabled)
		{
			glEnable(GL_TEXTURE_2D);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_2D, texture);

			glDisable(GL_CULL_FACE);
			shader->use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glEnable(GL_CULL_FACE);

			gui->RenderText(shaderText, "THE END" , SCR_WIDTH - 1250, SCR_HEIGHT - 200 , 2.5f, glm::vec3(1.0f, 1.0f, 1.0f));
			for (int i = 0; i < m_Buttons.size(); i++)
			{
				m_Buttons[i]->Render(shaderButton);
			}

			gui->RenderText(shaderText, m_Buttons[0]->GetText(), SCR_WIDTH - 1250 + 140, SCR_HEIGHT - 445 - 168, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, m_Buttons[1]->GetText(), SCR_WIDTH - 1250 + 220, SCR_HEIGHT - 445 - 272, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Statistics: ", SCR_WIDTH / 2 - 300, SCR_HEIGHT - 300.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Cut trees: " + std::to_string(RenderEngine::GetInstance()->GetCutTreesCounter()), SCR_WIDTH / 2 - 300, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Burnt trees: " + std::to_string(RenderEngine::GetInstance()->GetBurntTreesCounter()), SCR_WIDTH / 2 - 300, SCR_HEIGHT - 400.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, "Achievements: ", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 300.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			if ((RenderEngine::GetInstance()->GetBurntTreesCounter()) < 20)
			{
				gui->RenderText(shaderText, "Arsonist ", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else if ((RenderEngine::GetInstance()->GetBurntTreesCounter()) > 20 && RenderEngine::GetInstance()->GetBurntTreesCounter() < 25)
			{
				gui->RenderText(shaderText, "Advanced arsonist ", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else if (RenderEngine::GetInstance()->GetBurntTreesCounter() > 30)
			{
				gui->RenderText(shaderText, "Ultimate arsonist ", SCR_WIDTH / 2 + 100, SCR_HEIGHT - 350.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
			}

		}
	}

	void EndGameGUI::BackgroundInit()
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);


		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data = stbi_load(std::string("../Clumsy/src/models/endGame.jpg").c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			std::cout << "Ladowana tekstura" << std::endl;
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

	void EndGameGUI::HandleButtonClick(float screenX, float screenY, GLFWwindow* glfwWindow)
	{
		//after hitting Quit -> close the window of a game!
		//PlayAgain
		if (screenX > (m_Buttons[0]->GetCorner().x - (m_Buttons[0]->GetScale().x / 2)) && screenX < (m_Buttons[0]->GetCorner().x + (m_Buttons[0]->GetScale().x / 2))
			&& screenY < (m_Buttons[0]->GetCorner().y + m_Buttons[0]->GetScale().y) && screenY > m_Buttons[0]->GetCorner().y)
		{
			std::cout << "PLAY AGAIN BUTTON" << std::endl;
			m_Buttons[0]->OnClick();
			RenderEngine::GetInstance()->SetFirstLevel(true);
			CoreEngine::GetInstance()->SetGame(Clumsy::CoreEngine::GetInstance()->GetLevel1());
			m_Enabled = false;
		}
		//QUIT
		else if (screenX > (m_Buttons[1]->GetCorner().x - (m_Buttons[1]->GetScale().x / 2)) && screenX < (m_Buttons[1]->GetCorner().x + (m_Buttons[1]->GetScale().x / 2))
			&& screenY < (m_Buttons[1]->GetCorner().y + m_Buttons[1]->GetScale().y) && screenY > m_Buttons[1]->GetCorner().y)
		{
			std::cout << "QUIT BUTTON" << std::endl;
			glfwSetWindowShouldClose(glfwWindow, true);
			m_Buttons[1]->OnClick();
		}
	}
}