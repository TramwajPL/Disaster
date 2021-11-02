
#include "../pch.h"

#include <glm/gtx/string_cast.hpp>

#include "MousePicker.h"
#include "../Game/Enemy.h"
#include "../Game/Player.h"
#include "../GUI/StoreGUI.h"
#include "../Core/CoreEngine.h"
#include "../Game/TurnSystem.h"
#include "../GUI/WarehouseGUI.h"
#include "../Audio/AudioMaster.h"
#include "../GUI/DestructionBar.h"
#include "../GUI/BetweenLevelsGUI.h"
#include "../Components/RenderModelComponent.h"

namespace Clumsy
{
	void MousePicker::Update()
	{
		m_ViewMatrix = m_Camera->GetViewMatrix();
		m_CurrentRay = CalculateMouseRey();
	}

	glm::vec3 MousePicker::CalculateMouseRey()
	{
		double xpos, ypos;
		glfwGetCursorPos(m_Window->GetGLFWWindow(), &xpos, &ypos);
		glm::vec2 normalizedCoords = GetNormalisedDeviceCoordinates(xpos, ypos);
		glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
		glm::vec4 eyeCoords = ToEyeCoords(clipCoords);
		glm::vec3 worldRay = ToWorldCoords(eyeCoords);
		return worldRay;
	}

	glm::vec3 MousePicker::ToWorldCoords(glm::vec4 eyeCoords)
	{
		glm::mat4 invertedView = glm::inverse(m_ViewMatrix);
		glm::vec4 rayWorld = invertedView * eyeCoords;
		glm::vec3 mouseRay = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));
		return mouseRay;
	}

	glm::vec4 MousePicker::ToEyeCoords(glm::vec4 clipCoords)
	{
		glm::mat4 invertedProjection = glm::inverse(m_ProjectionMatrix);
		glm::vec4 eyeCoords = invertedProjection * clipCoords;
		return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	}

	glm::vec2 MousePicker::GetNormalisedDeviceCoordinates(float mouseX, float mouseY)
	{
		float x = (2.0f * mouseX) / m_Window->GetWidth() - 1;
		float y = (2.0f * mouseY) / m_Window->GetHeight() - 1;
		return glm::vec2(x, -y);
	}

	float MousePicker::CheckCollision(const Collider* aabb)
	{
		float t1 = (aabb->GetMinExtends().x - m_Camera->GetPosition().x) / m_CurrentRay.x;
		float t2 = (aabb->GetMaxExtends().x - m_Camera->GetPosition().x) / m_CurrentRay.x;
		float t3 = (aabb->GetMinExtends().y - m_Camera->GetPosition().y) / m_CurrentRay.y;
		float t4 = (aabb->GetMaxExtends().y - m_Camera->GetPosition().y) / m_CurrentRay.y;
		float t5 = (aabb->GetMinExtends().z - m_Camera->GetPosition().z) / m_CurrentRay.z;
		float t6 = (aabb->GetMaxExtends().z - m_Camera->GetPosition().z) / m_CurrentRay.z;

		float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
		float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

		if (tmax < 0.0f) 
		{
			//std::cout << "Collider behind camera" << std::endl;

			return -1.0f;
		}

		if (tmin > tmax) 
		{
			//std::cout << "No collision" << std::endl;
			return -1.0f;
		}

		if (tmin < 0.0f) 
		{
			//std::cout << "Collision detected" << std::endl;
			return tmax;
		}
		//std::cout << "Collision detected" << std::endl;
		return tmin;
	}

	glm::vec3 MousePicker::GetPickedObject(glm::vec3 originalPosition)
	{
		float checkCollisionResult;
		for (int i = 0; i < PhysicsEngine::GetInstance()->GetNumObjects() ; i++)
		{
			checkCollisionResult = CheckCollision(&PhysicsEngine::GetInstance()->GetObject(i).GetCollider());
			if (checkCollisionResult != -1) 
			{
				if (a == true) 
				{
					RenderEngine::GetInstance()->m_FirstInstructionTime = 0.5f;
					RenderEngine::GetInstance()->m_SecondInstructionTime = 10.5f;
					RenderEngine::GetInstance()->m_ThirdInstructionTime = 20.5f;
					a = false;
				}
				if (b == true && RenderEngine::GetInstance()->GetFirstLevel() == false) 
				{
					RenderEngine::GetInstance()->m_SecondLevelInstructionTime = 0.5f;
					b = false;
				}
				//vector of collider's position  
				glm::vec3 vectorGameObject = PhysicsEngine::GetInstance()->GetObject(i).GetPosition();
				for (int j = 0; j < RenderEngine::GetInstance()->map->GetAllChildren().size(); j++) 
				{
					glm::vec3 position = RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetTransform().GetPos();
					
					if (position == vectorGameObject && RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetM_Tag() == "shop") 
					{
						glm::vec3 destination = RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetPos();
						if (glm::length(originalPosition - destination) > 0.1f && glm::length(originalPosition - destination) < 1.5f)
						{
							RenderEngine::GetInstance()->GetStoreGUI()->SetActiveStore(RenderEngine::GetInstance()->map->GetAllChildren()[j]);
							Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetActivePlayer());
							RenderEngine::GetInstance()->GetStoreGUI()->SetActivePlayer(player);
							RenderEngine::GetInstance()->GetStoreGUI()->SetEnabled(true);
						}
					}
					if (position == vectorGameObject && RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetM_Tag() == "woodHouse") 
					{
						glm::vec3 destination = RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetPos();
						if (glm::length(originalPosition - destination) > 0.1f && glm::length(originalPosition - destination) < 1.5f)
						{
							Warehouse* warehouse = dynamic_cast<Warehouse*>(RenderEngine::GetInstance()->map->GetAllChildren()[j]);
							RenderEngine::GetInstance()->GetWarehouseGUI()->SetActiveWarehouse(warehouse);
							Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetActivePlayer());
							RenderEngine::GetInstance()->GetWarehouseGUI()->SetActivePlayer(player);
							RenderEngine::GetInstance()->GetWarehouseGUI()->SetEnabled(true);
						}
					}
					if (position == vectorGameObject && RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetM_Tag() == "win")
					{
						glm::vec3 destination = RenderEngine::GetInstance()->map->GetAllChildren()[j]->GetPos();
						if (glm::length(originalPosition - destination) > 0.1f && glm::length(originalPosition - destination) < 1.5f)
						{
							RenderEngine::GetInstance()->GetEndGameGUI()->SetEnabled(true);
							std::cout << "you win" << std::endl;
						}
					}
				}
				return PhysicsEngine::GetInstance()->GetObject(i).GetPosition();
			}
		}
		return originalPosition;
	}

	void MousePicker::HandleEvent(Event* event)
	{
		if (event->GetEventId() == "move")
		{
			Player* player = (Player*)event->GetParameter();
			RenderModelComponent* rmc = player->m_Rmc;
			glm::vec3* destination = &GetPickedObject(rmc->m_Transform.GetPos());
			glm::vec3* currentpos = &rmc->m_Transform.GetPos(); //pozycja playera
			if (glm::length(*currentpos - *destination) > 0.1f && glm::length(*currentpos - *destination) < 1.5f)
			{
				bool isThereATree = false;
				bool isThereEnemy = false;
				bool isTherePlayer = false;
				int t;
				for (int k = 0; k < RenderEngine::GetInstance()->treeTransforms.size(); k++)
				{
					if (*destination == RenderEngine::GetInstance()->treeTransforms[k].GetPos())
					{
						isThereATree = true;
						t = k;
						break;
					}
				}
				if (glm::length(RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.GetPos() - *destination) < 1.5f && RenderEngine::GetInstance()->enemy->getIfActive())
				{
					RenderEngine::GetInstance()->enemy->Fight();
				}

				// check if tile occupied
				for (int i = 0; i < TurnSystem::GetInstance()->GetPlayers().size(); i++)
				{
					Player* other = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetPlayers()[i]->GetGameObject());
					if (other)
					{
						if (glm::length(other->m_Rmc->m_Transform.GetPos() - *destination) < 0.8f)
						{
							isTherePlayer = true;
							RenderEngine::GetInstance()->m_TileOccupied = true;
							RenderEngine::GetInstance()->m_MoveFailTime = 0.8f;
							std::cout << "tile occupied" << std::endl;

							break;
						}
					}
				}
				if (!isTherePlayer)
				{
					if (!isThereATree)
					{
						glm::vec3 delta = ((GetPickedObject(rmc->m_Transform.GetPos()) - rmc->m_Transform.GetPos()) * glm::vec3(0.1f));
						float angle = 90.0f;
						glm::vec2 diff(destination->x - rmc->m_Transform.GetPos().x, destination->z - rmc->m_Transform.GetPos().z);
						float epsilon = 0.4f;
						if (diff.x > epsilon)
						{
							angle = -90.0f;
						}
						if (diff.y > epsilon)
						{
							angle -= 90.0f / angle * 45.0f;
						}
						else if (diff.y < -epsilon )
						{
							angle += 90.0f / angle * 45.0f;
						}
						rmc->SetAngle(angle);
						
						RenderEngine::GetInstance()->m_MovePlayer = true;
						RenderEngine::GetInstance()->SetDestination(*destination);
						RenderEngine::GetInstance()->SetCurrentPlayer(rmc);
						RenderEngine::GetInstance()->SetDeltaMove(delta);
						RenderEngine::GetInstance()->m_Movement = true;
						if (!RenderEngine::GetInstance()->GetWarehouseGUI()->IsEnabled() && !RenderEngine::GetInstance()->GetStoreGUI()->IsEnabled())
							player->IncrementActionCount();

						AudioMaster::GetInstance()->PlayWalk();
					}
					else if (isThereATree && player->IsIncrementingWoodCountPossible())
					{
						glm::vec3 delta = ((GetPickedObject(rmc->m_Transform.GetPos()) - rmc->m_Transform.GetPos()) * glm::vec3(0.1f));
						RenderEngine::GetInstance()->m_MovePlayer = true;
						RenderEngine::GetInstance()->SetDestination(*destination);
						RenderEngine::GetInstance()->SetCurrentPlayer(rmc);
						RenderEngine::GetInstance()->SetDeltaMove(delta);
						RenderEngine::GetInstance()->m_Movement = true;

						RenderEngine::GetInstance()->SetShakeTime(0.2f);
						RenderEngine::GetInstance()->GetPostProcessor()->m_Shake = true;

						player->IncrementActionCount();

						if (player->getCanBurn() == false) {
							RenderEngine::GetInstance()->wasCut = true;
							RenderEngine::GetInstance()->cutTreesTransforms.push_back(RenderEngine::GetInstance()->treeTransforms.at(t));
							RenderEngine::GetInstance()->treeTransforms.erase(RenderEngine::GetInstance()->treeTransforms.begin() + t);
							AudioMaster::GetInstance()->PlayChainsaw();
							RenderEngine::GetInstance()->SetCutTreesCounter(1);
						}
						else if (player->getCanBurn() == true) 
						{
							RenderEngine::GetInstance()->TreeBurnt();
							RenderEngine::GetInstance()->wasCut = true;
							RenderEngine::GetInstance()->treeTransforms.erase(RenderEngine::GetInstance()->treeTransforms.begin() + t);

							RenderEngine::GetInstance()->SetFireTreePosition(*destination); //FIRE
							RenderEngine::GetInstance()->isPlayed = true;
							AudioMaster::GetInstance()->PlayFire();
							RenderEngine::GetInstance()->SetBurntTreesCounter(1);

							if (RenderEngine::GetInstance()->GetFirstLevel() == true) {
								if ((RenderEngine::GetInstance()->GetBackgroundBar()->GetScale().x) > RenderEngine::GetInstance()->GetDestructionBar()->GetScale().x)
								{
									RenderEngine::GetInstance()->IncreaseScaleUp();
									if (RenderEngine::GetInstance()->GetBurntTrees() == 10) {
										RenderEngine::GetInstance()->GetBetweenLevelsGUI()->SetEnabled(true);
										RenderEngine::GetInstance()->SetFirstLevel(false);
										CoreEngine::GetInstance()->SetGame(CoreEngine::GetInstance()->GetLevel2());
									}
									
								}
							}
							for (int k = 0; k < RenderEngine::GetInstance()->ground.size(); k++)
							{
								if (*destination == RenderEngine::GetInstance()->ground[k].GetPos())
								{
									RenderEngine::GetInstance()->groundBurned.push_back(RenderEngine::GetInstance()->ground.at(k));
									RenderEngine::GetInstance()->ground.erase(RenderEngine::GetInstance()->ground.begin() + k);
								}
							}
						}

						if (RenderEngine::GetInstance()->enemy->GetIsDead() == true &&
							RenderEngine::GetInstance()->enemy->GetCondition() == true)
						{
							RenderEngine::GetInstance()->enemy->SetCountTrees(0);
							RenderEngine::GetInstance()->enemy->SetCondition(false);
						}

						RenderEngine::GetInstance()->enemy->IncrementCountTrees();

						RenderEngine::GetInstance()->enemy->checkIfRender(RenderEngine::GetInstance()->enemy->GetCountTrees());
						player->IncrementWoodCount();
						float angle = 90.0f;
						glm::vec2 diff(destination->x - rmc->m_Transform.GetPos().x, destination->z - rmc->m_Transform.GetPos().z);
						float epsilon = 0.4f;
						if (diff.x > epsilon)
						{
							angle = -90.0f;
						}
						if (diff.y > epsilon)
						{
							angle -= 90.0f / angle * 45.0f;
						}
						else if (diff.y < -epsilon)
						{
							angle += 90.0f / angle * 45.0f;
						}
						rmc->SetAngle(angle);
					}
					else if (isThereATree && !player->IsIncrementingWoodCountPossible())
					{
						// full wood
						RenderEngine::GetInstance()->m_TooMuchWood = true;
						RenderEngine::GetInstance()->m_MoveFailTime = 0.8f;
					}
					else if (isThereEnemy)
					{
						RenderEngine::GetInstance()->enemy->Fight();
						AudioMaster::GetInstance()->PlayChainsaw();

					}
					else
					{
						//std::cout << "Nie mozesz sie poruszyc " << std::endl;
					}
				}

				
			}
			else if (glm::length(*currentpos - *destination) >= 1.5f)
			{
				// too far
				RenderEngine::GetInstance()->m_MoveTooFar = true;
				RenderEngine::GetInstance()->m_MoveFailTime = 0.8f;
			}
		}

	}

}