#pragma once
#include "GUI.h"
#include "Button.h"
#include "glfw3.h"


namespace Clumsy {

	class CoreEngine;
	class EndGameGUI
	{
	public:
		EndGameGUI();
		~EndGameGUI();

		void Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT);
		void BackgroundInit();

		bool IsEnabled() { return m_Enabled; }
		void SetEnabled(bool doopy) { m_Enabled = doopy; }

		void HandleButtonClick(float screenX, float screenY, GLFWwindow* glfwWindow);

	private:
		std::vector<Button*> m_Buttons;
		//GUI* quitButton;
		//GUI* playAginButton;
		//GUI* theEndText;
		GUI* gui;
		unsigned int VBO, VAO, EBO;
		int width, height, nrChannels;
		unsigned int texture;
		bool m_Enabled = false;
	};
}