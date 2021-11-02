#pragma once
#include "../pch.h"
#include <cstdlib>
#include <glm/gtx/string_cast.hpp>
#include "../Game/TurnSystem.h"
#include "../Core/GameObject.h"

namespace Clumsy
{
	class Nature : public GameObject
	{
	public:
		Nature() : GameObject() 
        {
            srand(time(NULL));
            listOfPosition = new std::vector<glm::vec3>();
        }
		
        ~Nature() {}

		void SetTurnClass(TurnClass* tc) { m_TurnClass = tc; }

        void SetEnemyPreset(bool b) { enemyOnBoard = b; }

		void UpdateTurn()
		{
			isTurn = m_TurnClass->isTurn;
            if (!isTurn)
            {
                //To change number of spawned trees change these values
                callsController = rand() % (listOfPosition->size() - 1);
                callsController2 = 1;
            }
            if (isTurn)
            {
                std::cout << "turn... " << std::endl;
                if (Clumsy::RenderEngine::GetInstance()->enemy->getIfActive()) 
                {
                    // movement
                    EntMovement();
                    // respawn
                    RenderEngine::GetInstance()->ClearSpawnTreePositions();
					int randomNumber = (rand() % 5) + 1;
					for (int i = 0; i < randomNumber ; i++)
					{
						SpawnOneTreeEnemy();
					}                    
                }

                if (TurnSystem::GetInstance()->GetTurnCounter() % 2 == 0)
                {
                    SpawnOneTree();
                }
                std::cout << " Nature time! " << std::endl;
                std::cout << "Tura nr: " << TurnSystem::GetInstance()->GetTurnCounter() << std::endl;
                isTurn = false;
                m_TurnClass->isTurn = isTurn;
                m_TurnClass->wasTurnPrev = true;
            }
		}

        void SpawnOneTree()
        {
            if (RenderEngine::GetInstance()->cutTreesTransforms.size() > 0) 
            {
                int RandomTreeToSpawn = rand() % RenderEngine::GetInstance()->cutTreesTransforms.size();
                // check if tile occupied
                bool occupied = false;
                for (int i = 0; i < TurnSystem::GetInstance()->GetPlayers().size(); i++)
                {
                    Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetPlayers()[i]->GetGameObject());
                    if (player)
                    {
                        if (glm::length(player->m_Rmc->m_Transform.GetPos() - RenderEngine::GetInstance()->cutTreesTransforms[RandomTreeToSpawn].GetPos()) < 0.1f
                            || glm::length(RenderEngine::GetInstance()->GetDestination() - RenderEngine::GetInstance()->cutTreesTransforms[RandomTreeToSpawn].GetPos()) < 0.1f)
                        {
                            occupied = true;
                            std::cout << "bez drzewa!" << std::endl;
                            break;
                        }
                    }
                }
                if (!occupied)
                {
                    RenderEngine::GetInstance()->treeTransforms.push_back(RenderEngine::GetInstance()->cutTreesTransforms.at(RandomTreeToSpawn));
                    RenderEngine::GetInstance()->cutTreesTransforms.erase(RenderEngine::GetInstance()->cutTreesTransforms.begin() + RandomTreeToSpawn);
                }
            }
        }

		void SpawnOneTreeEnemy() {

			if (RenderEngine::GetInstance()->cutTreesTransforms.size() > 0) {
				int RandomTreeToSpawn = rand() % RenderEngine::GetInstance()->cutTreesTransforms.size();
                bool occupied = false;
                for (int i = 0; i < TurnSystem::GetInstance()->GetPlayers().size(); i++)
                {
                    Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetPlayers()[i]->GetGameObject());
                    if (player)
                    {
                        if (glm::length(player->m_Rmc->m_Transform.GetPos() - RenderEngine::GetInstance()->cutTreesTransforms[RandomTreeToSpawn].GetPos()) < 0.1f
                            || glm::length(RenderEngine::GetInstance()->GetDestination() - RenderEngine::GetInstance()->cutTreesTransforms[RandomTreeToSpawn].GetPos()) < 0.1f)
                        {
                            occupied = true;
                            std::cout << "bez drzewa!" << std::endl;
                            break;
                        }
                    }
                }
				bool burned = false;
				for (int i = 0; i < RenderEngine::GetInstance()->groundBurned.size(); i++)
				{
					if (glm::length(RenderEngine::GetInstance()->cutTreesTransforms[RandomTreeToSpawn].GetPos() -
						RenderEngine::GetInstance()->groundBurned[i].GetPos()) < 0.1f )
					{
						burned = true;
					}
				}
				if (!burned && !occupied)
				{
					RenderEngine::GetInstance()->treeTransforms.push_back(RenderEngine::GetInstance()->cutTreesTransforms.at(RandomTreeToSpawn));
					RenderEngine::GetInstance()->SetSpawnTreePosition(RenderEngine::GetInstance()->cutTreesTransforms.at(RandomTreeToSpawn));
					RenderEngine::GetInstance()->enemySpawn = true;
					RenderEngine::GetInstance()->cutTreesTransforms.erase(RenderEngine::GetInstance()->cutTreesTransforms.begin() + RandomTreeToSpawn);
				}
				
			}
		}

        void EntMovement()
        {
           for (int i = 0; i < 5; i++)
           {
               if (EnemyMoved(RenderEngine::GetInstance()->groundSand))
               {
				   RenderEngine::GetInstance()->m_MoveEnemy = true;
                   return;
               }
               else if (EnemyMoved(RenderEngine::GetInstance()->cutTreesTransforms))
               {
				   RenderEngine::GetInstance()->m_MoveEnemy = true;
                   return;
               }
               else if (EnemyMoved(RenderEngine::GetInstance()->groundBurned))
               {
				   RenderEngine::GetInstance()->m_MoveEnemy = true;
                   return;
               }
           }
        }

        bool EnemyMoved(std::vector<Transform> vec)
        {
            if (vec.size() > 0)
            {
                int number = rand() % vec.size();
                glm::vec3* destination = &vec[number].GetPos();
                std::cout << "destination ent " << glm::to_string(*destination) << std::endl;
                glm::vec3* currentpos = &RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.GetPos();
                std::cout << "position ent " << glm::to_string(*currentpos) << std::endl;
                if (glm::length(*currentpos - *destination) < 2.5f)
                {
                    // move
                    glm::vec3 delta = (*destination - *currentpos) * glm::vec3(0.1f);
					float angle = 90.0f;
					glm::vec2 diff(destination->x - RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.GetPos().x, 
						destination->z - RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.GetPos().z);
					float epsilon = 0.1f;
					if (diff.x > epsilon)
					{
						angle = -90.0f;
						std::cout << "Weszlo 1" << std::endl;
					}
					if (diff.y > epsilon)
					{
						angle -= 90.0f / angle * 45.0f;
						std::cout << "Weszlo 2" << std::endl;
					}
					else if (diff.y < -epsilon)
					{
						angle += 90.0f / angle * 45.0f;
						std::cout << "Weszlo 3" << std::endl;
					}
					RenderEngine::GetInstance()->enemy->m_Rmc->SetAngle(angle);
                    Clumsy::RenderEngine::GetInstance()->SetEnemyDestination(*destination);
                    Clumsy::RenderEngine::GetInstance()->SetEnemyDeltaMove(delta);
                    Clumsy::RenderEngine::GetInstance()->m_EnemyMovement = true;
                    return true;
                }
            }
            return false;
        }
        
	private:
		bool isTurn = false;
		bool enemyOnBoard = false;
		TurnClass* m_TurnClass = nullptr;

        int callsController = 0;
        int callsController2 = 1;
        std::vector<glm::vec3>* listOfPosition;
	};
}