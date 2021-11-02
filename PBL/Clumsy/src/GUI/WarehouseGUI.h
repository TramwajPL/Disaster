#pragma once

#include <vector>

#include "GUI.h"
#include "Button.h"
#include "../Game/Player.h"
#include "../Game/Warehouse.h"

namespace Clumsy
{
	class WarehouseGUI
	{
	public:
		WarehouseGUI();
		~WarehouseGUI() {}
		void Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT);
		void BackgroundInit();
		void SetActiveWarehouse(Warehouse* warehouse) { m_Warehouse = warehouse; }
		void SetActivePlayer(Player* player) { m_Player = player; }
		bool IsEnabled() { return m_Enabled; }
		void SetEnabled(bool doopy) { m_Enabled = doopy; }

		void Update(float deltaTime);
		void HandleButtonClick(float screenX, float screenY);

	private:
		std::vector<Button*> m_Buttons;
		GUI* gui;
		Button* button;
		unsigned int VBO, VAO, EBO, texture;
		int width, height, nrChannels;
		bool m_Enabled = false;
		Warehouse* m_Warehouse;
		Player* m_Player;
	};
}