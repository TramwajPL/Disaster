#pragma once

#include "Game.h"
#include "../Game/Enemy.h"
#include "../EventSystem/EventSystem.h"
#include "../RenderEngine/RenderEngine.h"


namespace Clumsy
{
	class CoreEngine
	{
	public:
		void Start();
		void Stop();


		void SetGame(Game* game) 
		{
			RenderEngine::GetInstance()->m_SecondLevelInstruction = true;
			RenderEngine::GetInstance()->SetBurntToZero();
			TurnSystem::GetInstance()->SetTurnCounter(0);
			RenderEngine::GetInstance()->enemy->SetIsDead(false);
			RenderEngine::GetInstance()->enemy->SetRenderEnemy(false);
			m_Game->getRoot().DeleteAll();
			RenderEngine::GetInstance()->m_Cubes.clear();
			TurnSystem::GetInstance()->DeletePlayers();
			PhysicsEngine::GetInstance()->m_Objects.clear();
			RenderEngine::GetInstance()->treeTransforms.clear();
			RenderEngine::GetInstance()->cutTreesTransforms.clear();
			RenderEngine::GetInstance()->ground.clear();
			RenderEngine::GetInstance()->groundBurned.clear();
			RenderEngine::GetInstance()->groundSand.clear();
			RenderEngine::GetInstance()->enemy->SetCountTrees(0);

			EventSystem::GetInstance()->UnsubscribeListener("hire", m_Game);
			EventSystem::GetInstance()->SubscribeListener("hire", game);

			m_Game = game;
			m_Game->Init();
			if (!RenderEngine::GetInstance()->GetFirstLevel())
			{
				RenderEngine::GetInstance()->SetScaleUp(0.0f);
				RenderEngine::GetInstance()->SetXScaleBackgroundDefault(0.0f);
			}
			else
			{
				RenderEngine::GetInstance()->SetScaleUp(0.0f);
			}

		}

		Game* GetGame() { return m_Game; }
		Game* GetLevel2() { return m_Level2; }
		Game* GetLevel1() { return m_Level1; }
		
		static void CreateInstance(float frameRate, Window* window, Game* game, Game* level2)
		{
			assert(!m_Instance);
			m_Instance = new CoreEngine(frameRate, window, game, level2);
		}

		static CoreEngine* GetInstance()
		{
			assert(m_Instance);
			return m_Instance;
		}

		bool m_IsRunning;

	private:
		CoreEngine(float frameRate, Window* window, Game* game, Game* level2) :
			m_IsRunning(false),
			m_FrameTime(1.0f / frameRate),
			m_Window(window),
			m_Game(game),
			m_Level2(level2),
			m_Level1(game)
		{
			m_Game->Init();
			EventSystem::GetInstance()->SubscribeListener("hire", m_Game);
		}

		float m_FrameTime = 0.0f;		//How long, in seconds, one frame should take
		Window* m_Window;				//Used to display the game
		Game* m_Game;					//The game itself. Stored as pointer so the user can pass in a derived class.
		Game* m_Level2;					
		Game* m_Level1;
		static CoreEngine* m_Instance;
	};
}