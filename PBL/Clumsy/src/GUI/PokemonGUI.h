#pragma once

#include <vector>
#include "glfw3.h"
#include "GUI.h"
#include "Button.h"
#include "DestructionBar.h"

namespace Clumsy 
{
	class PokemonGUI 
	{
	public:
		enum BattleState { START, PLAYERTURN, ENEMYTURN, WON, LOST };

		PokemonGUI(Shader* buttonShader);
		~PokemonGUI();

		void Render(Shader* shader, Shader* shaderButton, Shader* shaderText, int SCR_WIDTH, int SCR_HEIGHT);
		void BackgroundInit();

		bool IsEnabled() { return m_Enabled; }
		void SetEnabled(bool doopy) { m_Enabled = doopy; }

		void HandleButtonClick(float screenX, float screenY);
		void HandleBattle();
		BattleState getBattleState() { return m_BattleState; }
		void StartBattle() { m_BattleState = START; }
		void Reset();

		glm::vec3 scaleEnemy{ 0.0, 0.03f, 0.3f };
		glm::vec3 scalePlayer{ 0.0, 0.03f, 0.3f };
		std::string m_textString;
		bool m_BattleCommences = false;
	private:
		bool m_AttackButtonClickable = false;
		bool m_HealButtonClickable = false;
		int m_PlayerHP = 100, m_EnemyHp = 100, m_AttackValue = 50, m_HealValue = 50, m_EnemyAttackValue = 10;
		int m_SCRWIDTH;
		int m_SCRHEIGHT;
		Shader* m_Shader;
		BattleState m_BattleState = START;
		std::vector<Button*> m_Buttons;
		GUI* gui;
		unsigned int VBO, VAO, EBO;
		int width, height, nrChannels;
		unsigned int texture;
		bool m_Enabled = false;
		DestructionBar* backEnemy;
		DestructionBar* backPlayer;
		DestructionBar* Enemy;
		DestructionBar* Player;
	};
}