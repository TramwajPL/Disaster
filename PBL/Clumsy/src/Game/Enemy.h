#pragma once
#include "../Core/GameObject.h"
#include "../RenderEngine/Model.h"
#include "../RenderEngine/RenderEngine.h"
#include "../GUI/PokemonGUI.h"
#include "../GUI/EndGameGUI.h"
#include "../Audio/AudioMaster.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace Clumsy {

	class Enemy : public GameObject
	{
	public:
		Enemy(Model* modelInitial, Transform transform, int maxCollectedTrees = 3) :
			m_ModelInitial(modelInitial), m_MaxCollectedTrees(maxCollectedTrees), GameObject(transform)
		{
		}
		Enemy() {}

		void SetM_Tag(std::string tag) override { m_Tag = tag; }
		bool GetRenderEnemy() override { return m_RenderEnemy; }
		void SetRenderEnemy(bool renderEnemy) { m_RenderEnemy = renderEnemy; }

		void checkIfRender(int collectedTrees) override
		{
			int randomNumber = (rand() % 20) + 1; 
			if (collectedTrees > randomNumber && collectedTrees > 10)
			{
				m_IsActive = true;
				isEnemyDead = false;;
				m_RenderEnemy = true;
				if (shouldShoutBePlayed) {
					AudioMaster::GetInstance()->PlayShout();
					shouldShoutBePlayed = false;
				}
			}
			else if (collectedTrees > 10 && randomNumber > collectedTrees)
			{
				m_IsActive = true;
				isEnemyDead = false;;
				m_RenderEnemy = true;
				if (shouldShoutBePlayed) {
					AudioMaster::GetInstance()->PlayShout();
					shouldShoutBePlayed = false;
				}
			}
		}

		void Fight()
		{
			RenderEngine::GetInstance()->GetPokemonGUI()->SetEnabled(true);
			RenderEngine::GetInstance()->GetPokemonGUI()->Reset();
			RenderEngine::GetInstance()->GetPokemonGUI()->m_BattleCommences = true;
			std::cout << "FIGHT!" << std::endl;
			shouldShoutBePlayed = true;
		}

		void Die()
		{
			if (RenderEngine::GetInstance()->GetPokemonGUI()->getBattleState() == PokemonGUI::BattleState::WON)
			{
				std::cout << "umrzyj" << std::endl;
				m_IsActive = false;
				isEnemyDead = true;
				m_Condition =  true;
			}
		}

		void Loose()
		{
			if (RenderEngine::GetInstance()->GetPokemonGUI()->getBattleState() == PokemonGUI::BattleState::LOST)
			{
				RenderEngine::GetInstance()->GetEndGameGUI()->SetEnabled(true);
			}
		}

		bool GetIsDead() override { return isEnemyDead; }
		void SetIsDead(bool dead) override { isEnemyDead = dead; }
		bool GetCondition() { return m_Condition; }
		void SetCondition(bool condition) { m_Condition = condition; }
		void SetShouldBeDead(bool test) { m_ShouldBeDead = test; }
		bool GetShouldBeDead() { return m_ShouldBeDead; }

		bool getIfActive() { return m_IsActive;  }
		void SetCountTrees(int i) { countTrees = i; }
		void IncrementCountTrees() { countTrees++; }
		int GetCountTrees() {
			return countTrees;
		}

	private:
		int m_MaxCollectedTrees;
		bool m_Condition = false;
		Model* m_ModelInitial;
		bool m_ShouldBeDead = false;
		bool m_IsActive = false;
		int countTrees = 0;
		bool shouldShoutBePlayed = true;
	};
}