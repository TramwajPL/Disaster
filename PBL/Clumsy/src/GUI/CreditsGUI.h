#pragma once

#include "glfw3.h"

#include "GUI.h"
#include "Button.h"

namespace Clumsy 
{

	class CreditsGUI 
	{

	public:
		CreditsGUI();
		~CreditsGUI();
		void Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT);
		void BackgroundInit();

		bool IsEnabled() { return m_Enabled; }
		void SetEnabled(bool doopy) { m_Enabled = doopy; }

		void HandleButtonClick(float screenX, float screenY);
	private:
		Button* b2;
		std::vector<Button*> m_Buttons;
		GUI* gui;
		unsigned int VBO, VAO, EBO;
		int width, height, nrChannels;
		unsigned int texture;
		bool m_Enabled = false;
	};
}