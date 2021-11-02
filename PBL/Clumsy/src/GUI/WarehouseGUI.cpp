
#include "../pch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "WarehouseGUI.h"
#include "../Game/TurnSystem.h"
#include "../RenderEngine/Shader.h"

namespace Clumsy
{
	WarehouseGUI::WarehouseGUI()
	{
		BackgroundInit();

		// Buttons
		Button* exitButton = new Button(glm::vec2(0.36f, 0.33f), "X", glm::vec3(0.f, 0.0f, 0.0f), glm::vec2(0.07f, 0.1f));
		m_Buttons.push_back(exitButton);
		Button* storeWoodButton = new Button(glm::vec2(-0.27f, 0.1f), "Store", glm::vec3(0.f, 0.0f, 0.0f), glm::vec2(0.19f, 0.1f));
		m_Buttons.push_back(storeWoodButton);
		Button* withdrawWoodButton = new Button(glm::vec2(-0.27f, -0.09f), "Withdraw", glm::vec3(0.f, 0.0f, 0.0f), glm::vec2(0.19f, 0.1f));
		m_Buttons.push_back(withdrawWoodButton);

		button = new Button(glm::vec2(0.13f, 0.0f), "", glm::vec3(0.f, 0.0f, 0.0f), glm::vec2(0.25f, 0.1f));

		gui = new GUI();
	}

	void WarehouseGUI::Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT)
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

			// Render buttons
			for (int i = 0; i < m_Buttons.size(); i++)
			{
				m_Buttons[i]->Render(shaderButton);
			}

			button->Render(shaderButton);

			// Render text
			gui->RenderText(shaderText, "Wood stored: " + std::to_string(m_Warehouse->GetWoodStored()), SCR_WIDTH - 950.0f, SCR_HEIGHT - 550.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(shaderText, m_Buttons[0]->GetText(), SCR_WIDTH - 625.0f, SCR_HEIGHT - 375.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			for (int i = 1; i < m_Buttons.size(); i++)
			{
				gui->RenderText(shaderText, m_Buttons[i]->GetText(), SCR_WIDTH - 1300, SCR_HEIGHT - 400 - 100 * i, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}

	void WarehouseGUI::BackgroundInit()
	{

		float vertices[] = {
			// positions          // colors           // texture coords
			0.4f, 0.4f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			0.4f, -0.4f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.4f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.4f,  0.4f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
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
		unsigned char* data = stbi_load(std::string("../Clumsy/src/models/wood2.jpg").c_str(), &width, &height, &nrChannels, 0);
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

	void WarehouseGUI::HandleButtonClick(float screenX, float screenY)
	{		
		if (screenX > (m_Buttons[0]->GetCorner().x - (m_Buttons[0]->GetScale().x / 2)) && screenX < (m_Buttons[0]->GetCorner().x + (m_Buttons[0]->GetScale().x / 2))
			&& screenY < (m_Buttons[0]->GetCorner().y + m_Buttons[0]->GetScale().y) && screenY > m_Buttons[0]->GetCorner().y)
		{
			m_Buttons[0]->OnClick();
			m_Enabled = false;			
			m_Player->IncrementActionCount();
		}
		else if (screenX > (m_Buttons[1]->GetCorner().x - (m_Buttons[1]->GetScale().x / 2)) && screenX < (m_Buttons[1]->GetCorner().x + (m_Buttons[1]->GetScale().x / 2))
			&& screenY < (m_Buttons[1]->GetCorner().y + m_Buttons[1]->GetScale().y) && screenY > m_Buttons[1]->GetCorner().y)
		{
			m_Buttons[1]->OnClick();
			m_Buttons[1]->m_EffectTime = 0.2f;
			Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetActivePlayer());
			if (player && player->IsDecrementingWoodCountPossible())
			{
				m_Warehouse->StorePieceOfWood();
				player->DecrementWoodCount();
				m_Buttons[1]->m_Ok = true;
			}
			else
			{
				m_Buttons[1]->m_Fail = true;
			}
		}
		else if (screenX > (m_Buttons[2]->GetCorner().x - (m_Buttons[2]->GetScale().x / 2)) && screenX < (m_Buttons[2]->GetCorner().x + (m_Buttons[2]->GetScale().x / 2))
			&& screenY < (m_Buttons[2]->GetCorner().y + m_Buttons[2]->GetScale().y) && screenY > m_Buttons[2]->GetCorner().y)
		{
			m_Buttons[2]->OnClick();
			m_Buttons[2]->m_EffectTime = 0.2f;
			Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetActivePlayer());
			if (player && player->IsIncrementingWoodCountPossible() && m_Warehouse->IsThereAnyWood())
			{
				m_Warehouse->WithdrawPieceOfWood();
				player->IncrementWoodCount();
				m_Buttons[2]->m_Ok = true;
			}
			else
			{
				m_Buttons[2]->m_Fail = true;
			}
		}
	}

	void WarehouseGUI::Update(float deltaTime)
	{
		for (int i = 1; i < m_Buttons.size(); i++)
		{
			m_Buttons[i]->Update(deltaTime);
		}
	}
}