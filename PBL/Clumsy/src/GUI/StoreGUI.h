#pragma once

#include "GUI.h"
#include "Button.h"

#include "../Game/Player.h"

namespace Clumsy
{
	class StoreGUI
	{
	public:
		StoreGUI();
		~StoreGUI();
		void Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT);
		void BackgroundInit();
		void SetActiveStore(GameObject* store) { m_Store = store; }
		void SetActivePlayer(Player* player) { m_Player = player; }
		bool IsEnabled() { return m_Enabled; }
		void SetEnabled(bool doopy) { m_Enabled = doopy; }

		GameObject* GetActiveStore() { return m_Store; }
		void HandleButtonClick(float screenX, float screenY);
		void Update(float deltaTime);

	private:
		std::vector<Button*> m_Buttons;
		GUI* gui;
		unsigned int VBO, VAO, EBO, texture;
		int width, height, nrChannels;
		bool m_Enabled = false;

		GameObject* m_Store;
		Player* m_Player;
	};
}
