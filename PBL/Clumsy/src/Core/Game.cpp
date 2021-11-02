#include "../pch.h"

#include <yaml.h>

#include "Game.h"
#include "../Game/Enemy.h"
#include "../Game/Warehouse.h"
#include "../GUI/PokemonGUI.h"
#include "../GUI/EndGameGUI.h"
#include "../Game/TurnSystem.h"
#include "../Game/TreeObject.h"
#include "../GUI/WarehouseGUI.h"
#include "../Audio/AudioMaster.h"
#include "../GUI/BetweenLevelsGUI.h"
#include "../Components/RenderModelComponent.h"
#include "../Components/RenderInstancedGround.h"
#include "../Components/PhysicsObjectComponent.h"
#include "../Components/RenderInstancedModelComponent.h"
#include "../Components/RenderInstancedTreesComponent.h"
#include "../Components/RenderInstancedBurnedGroundComponent.h"

namespace Clumsy 
{	
	void Game::Render()
	{
		if (RenderEngine::GetInstance()->GetPokemonGUI()->IsEnabled() == true) 
		{
			RenderEngine::GetInstance()->RenderPokemonGUI();
		}
		else if (RenderEngine::GetInstance()->GetMenuGUI()->IsEnabled() == true) 
		{
			RenderEngine::GetInstance()->RenderMainMenu();
		}
		else if (RenderEngine::GetInstance()->GetEndGameGUI()->IsEnabled() == true)
		{
			RenderEngine::GetInstance()->RenderEndGame();
		}
		else if (RenderEngine::GetInstance()->GetCreditsGUI()->IsEnabled() == true) 
		{
			RenderEngine::GetInstance()->GetMenuGUI()->SetEnabled(false);
			RenderEngine::GetInstance()->RenderCreditsGUI();
		}
		else if (RenderEngine::GetInstance()->GetBetweenLevelsGUI()->IsEnabled() == true) 
		{
			RenderEngine::GetInstance()->RenderBetweenLevelsGUI();
		}
		else 
		{	
			//RenderEngine::GetInstance()->GetPostProcessor()->BeginRender();
			RenderEngine::GetInstance()->Render(m_Root); 
			//RenderEngine::GetInstance()->GetPostProcessor()->EndRender();
			//RenderEngine::GetInstance()->GetPostProcessor()->Render(glfwGetTime());
			RenderEngine::GetInstance()->RenderGUI();
			if (RenderEngine::GetInstance()->GetPokemonGUI()->IsEnabled() == true) {
				RenderEngine::GetInstance()->RenderPokemonGUI();
			}
		}
	}

	void Game::Update(float deltaTime)
	{
		TurnSystem::GetInstance()->Update();
		m_Root.UpdateAll();
		RenderEngine::GetInstance()->UpdateCubes();
		// gui buttons change colors
		RenderEngine::GetInstance()->GetWarehouseGUI()->Update(deltaTime);
		RenderEngine::GetInstance()->GetStoreGUI()->Update(deltaTime);
		// move fail
		if (RenderEngine::GetInstance()->m_MoveFailTime > 0.0f)
		{
			RenderEngine::GetInstance()->m_MoveFailTime -= deltaTime;
			if (RenderEngine::GetInstance()->m_MoveFailTime <= 0.0f)
			{
				Clumsy::RenderEngine::GetInstance()->m_MoveTooFar = false;
				Clumsy::RenderEngine::GetInstance()->m_TooMuchWood = false;
				Clumsy::RenderEngine::GetInstance()->m_TileOccupied = false;
			}
		}
		//first instruction
		if (RenderEngine::GetInstance()->m_ThirdInstructionTime > 0.0f)
		{
			RenderEngine::GetInstance()->m_FirstInstructionTime -= deltaTime;
			RenderEngine::GetInstance()->m_SecondInstructionTime -= deltaTime;
			RenderEngine::GetInstance()->m_ThirdInstructionTime -= deltaTime;
			if (RenderEngine::GetInstance()->m_FirstInstructionTime <= 0.0f)
			{
				Clumsy::RenderEngine::GetInstance()-> m_FirstInstruction = false;
			} 
			if (RenderEngine::GetInstance()->m_SecondInstructionTime <= 0.0f) {
				Clumsy::RenderEngine::GetInstance()->m_SecondInstruction = false;
			}	
			if (RenderEngine::GetInstance()->m_ThirdInstructionTime <= 0.0f) {
				Clumsy::RenderEngine::GetInstance()->m_ThirdInstruction = false;
			}
		}


		//Instruction for level 2
		if (RenderEngine::GetInstance()->m_SecondLevelInstructionTime > 0.0f)
		{
			RenderEngine::GetInstance()->m_SecondLevelInstructionTime -= deltaTime;
			if (RenderEngine::GetInstance()->m_SecondLevelInstructionTime <= 0.0f)
			{
				Clumsy::RenderEngine::GetInstance()->m_SecondLevelInstruction = false;
			}
		}

		// shake
		if (RenderEngine::GetInstance()->GetShakeTime() > 0.0f)
		{
			RenderEngine::GetInstance()->SetShakeTime(RenderEngine::GetInstance()->GetShakeTime() - deltaTime);
			if (RenderEngine::GetInstance()->GetShakeTime() <= 0.0f)
				Clumsy::RenderEngine::GetInstance()->GetPostProcessor()->m_Shake = false;
		}
		//movement
		if (RenderEngine::GetInstance()->m_Movement)
		{
			glm::vec3 currentPos = RenderEngine::GetInstance()->GetCurrentPlayer()->m_Transform.GetPos();
			glm::vec3 delta = RenderEngine::GetInstance()->GetDeltaMove();
			glm::vec3 destination = RenderEngine::GetInstance()->GetDestination();
			if (glm::length(currentPos - destination) > 0.001f)
			{
				glm::vec3 pos = RenderEngine::GetInstance()->GetCurrentPlayer()->m_Transform.GetPos();
				glm::vec3 newpos = pos + delta;
				RenderEngine::GetInstance()->GetCurrentPlayer()->m_Transform.SetPos(newpos);
				if (glm::length(pos - destination) <= 0.001f)
				{
					RenderEngine::GetInstance()->GetCurrentPlayer()->m_Transform.SetPos(destination);
					RenderEngine::GetInstance()->m_Movement = false;
				}
			}
		}
		//enemy movement
		if (RenderEngine::GetInstance()->m_EnemyMovement)
		{
			glm::vec3 currentPos = RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.GetPos();
			glm::vec3 delta = RenderEngine::GetInstance()->GetEnemyDeltaMove();
			glm::vec3 destination = RenderEngine::GetInstance()->GetEnemyDestination();
			if (glm::length(currentPos - destination) > 0.001f)
			{
				glm::vec3 pos = RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.GetPos();
				glm::vec3 newpos = pos + delta;
				RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.SetPos(newpos);
				if (glm::length(pos - destination) <= 0.001f)
				{
					RenderEngine::GetInstance()->enemy->m_Rmc->m_Transform.SetPos(destination);
					RenderEngine::GetInstance()->m_EnemyMovement = false;
				}
			}
		}
		if (RenderEngine::GetInstance()->GetPokemonGUI()->m_BattleCommences) 
		{
			RenderEngine::GetInstance()->GetPokemonGUI()->HandleBattle();
		}
	}

	void Game::HandleEvent(Event* event)
	{
		if (event->GetEventId() == "hire")
		{
			glm::vec3 shopPos(RenderEngine::GetInstance()->GetStoreGUI()->GetActiveStore()->GetTransform().GetPos());
			glm::quat rotBoy = glm::angleAxis(glm::radians(-180.f), glm::vec3(1.0f, 0.0f, 0.0f));
			rotBoy = glm::rotate(rotBoy, glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f) * rotBoy);
			Transform transform(shopPos, rotBoy, 0.004f);
			Player* boy = new Player(transform);
			PhysicsObject* ob = new PhysicsObject(
				new BoundingSphere(boy->GetTransform().GetPos(), 0.1f), &boy->GetTransform());
			PhysicsEngine::GetInstance()->AddObject(*ob);
			RenderModelComponent* rmc = new RenderModelComponent(RenderEngine::GetInstance()->playerModel, boy->GetTransform(), 360.0f);
			boy->m_Rmc = rmc;
			Clumsy::Cube* c1 = new Clumsy::Cube(transform);
			c1->SetPlayer(rmc);
			RenderEngine::GetInstance()->AddCube(c1);

			AddToScene((boy)->AddComponent(rmc));
			boy->AddComponent(new PhysicsObjectComponent(ob));
			TurnSystem::GetInstance()->AddPlayer(boy);			
		}
	}

	void Game::AddToScene(GameObject* child)
	{
		m_Root.AddChild(child);
	}

    std::string path = "../Clumsy/src/models/";

    void Game::SceneParser(GameObject* map, std::string filename)
    {
		AddToScene(map);
		std::vector<Transform> allTransformsM3;
		std::vector<Transform> allTransformsM4;
		std::vector<Transform> allTransformsM5;
		std::vector<Transform> allTransformsM6;
		bool model3 = false;
		bool model4 = false;
		bool model5 = false;
		bool model6 = false;
        std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(filename);
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]["PrefabInstance"]) {
				if (nodes[i]["PrefabInstance"]["m_Modification"]) {
					if (nodes[i]["PrefabInstance"]["m_Modification"]["m_Modifications"]) {
						bool firstNode = true;
						std::vector<float> values;
						std::string k;
						Transform transform;
						Transform transform2;
						for (YAML::iterator it = nodes[i]["PrefabInstance"]["m_Modification"]["m_Modifications"].begin(); it != nodes[i]["PrefabInstance"]["m_Modification"]["m_Modifications"].end(); ++it) {
							if (firstNode) {
								if (it->first.as<std::string>() == "value") {
									k = it->second.as<std::string>();
									if (k.find("Pasture") != std::string::npos) {
										model3 = true;
									}
									if (k.find("Desert") != std::string::npos) {
										model4 = true;
									}
									if (k.find("Stone") != std::string::npos) {
										model5 = true;
									}
									if (k.find("Water") != std::string::npos) {
										model6 = true;
									}
									firstNode = false;
								}
							}
							else {
								if (it->first.as<std::string>() == "value") {
									values.push_back(it->second.as<float>());
								}
							}
							
						} 
						values.pop_back();
						values.pop_back();
						values.pop_back();
						values.pop_back();

						if (model3) {
							transform.SetPosX(values[0]);
							transform.SetPosY(values[1]);
							transform.SetPosZ(values[2]);
							transform.SetRotX(0.0f);
							transform.SetRotY(0.7f);
							transform.SetRotZ(0.7f);
							transform.SetRotW(0.0f);
							transform.SetScale(0.0001f);

							RenderEngine::GetInstance()->ground.push_back(transform);
							allTransformsM3.push_back(transform);
							glm::vec3 min = glm::vec3(transform.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
							glm::vec3 max = glm::vec3(transform.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
							PhysicsObject* pO = new PhysicsObject(new Aabb(min, max), &transform);
							PhysicsEngine::GetInstance()->AddObject(*pO);
							GameObject* pasture = new GameObject(transform);
							map->AddChild((pasture)->AddComponent(new PhysicsObjectComponent(pO)));

							transform2.SetPosX(values[0]);
							transform2.SetPosY(values[1]);
							transform2.SetPosZ(values[2]);
							transform2.SetRotX(0.0f);//0
							transform2.SetRotY(0.7f);//0
							transform2.SetRotZ(0.7f);//0
							transform2.SetRotW(0.0f);//1

							transform2.SetScale(0.08f);
							float i;
							i = (rand() % 8) + 1;
							i /= 100.0f;
							i += transform2.GetScale();
							transform2.SetScale(i);
							
							allTransformsM7.push_back(transform2);
							RenderEngine::GetInstance()->treeTransforms.push_back(transform2);

							glm::vec3 min2 = glm::vec3(transform2.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
							glm::vec3 max2 = glm::vec3(transform2.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
							PhysicsObject* pO2 = new PhysicsObject(new Aabb(min2, max2), &transform2);
							PhysicsEngine::GetInstance()->AddObject(*pO2);
							TreeObject* tree = new TreeObject(transform2);
							tree->SetM_Tag("tree");
							map->AddChild((tree)->AddComponent(new PhysicsObjectComponent(pO2)));
							model3 = false;
						}

						if (model4) {
							transform.SetPosX(values[0]);
							transform.SetPosY(values[1]);
							transform.SetPosZ(values[2]);
							transform.SetRotX(0.0f);//0
							transform.SetRotY(0.7f);//0
							transform.SetRotZ(0.7f);//0
							transform.SetRotW(0.0f);//1
							transform.SetScale(0.0001f);

							RenderEngine::GetInstance()->groundSand.push_back(transform);
							allTransformsM4.push_back(transform);
							glm::vec3 min = glm::vec3(transform.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
							glm::vec3 max = glm::vec3(transform.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
							PhysicsObject* pO = new PhysicsObject(new Aabb(min, max), &transform);
							PhysicsEngine::GetInstance()->AddObject(*pO);
							map->AddChild((new Clumsy::GameObject(transform))
								->AddComponent(new PhysicsObjectComponent(pO)));
							model4 = false;
						}
						if (model5) {
							transform.SetPosX(values[0]);
							transform.SetPosY(values[1]);
							transform.SetPosZ(values[2]);
							transform.SetRotX(0.0f);//0
							transform.SetRotY(0.7f);//0
							transform.SetRotZ(0.7f);//0
							transform.SetRotW(0.0f);//1
							transform.SetScale(0.0001f);
							allTransformsM5.push_back(transform);
							map->AddChild(new Clumsy::GameObject(transform));
							model5 = false;
						}
						if (model6) {
							transform.SetPosX(values[0]);
							transform.SetPosY(values[1]);
							transform.SetPosZ(values[2]);
							transform.SetRotX(0.0f);//0
							transform.SetRotY(0.7f);//0
							transform.SetRotZ(0.7f);//0
							transform.SetRotW(0.0f);//1
							transform.SetScale(0.0001f);

							allTransformsM6.push_back(transform);
							map->AddChild(new Clumsy::GameObject(transform));
							model6 = false;
						}
					}
				}
            }
        }

		Transform transformShop;
		Transform transformWoodHouse;

		transformShop.SetPosX(1.19f);
		transformShop.SetPosY(-1.0f);
		transformShop.SetPosZ(1.52f);
		transformShop.SetRotX(0.0f);//0
		transformShop.SetRotY(0.7f);//0
		transformShop.SetRotZ(0.7f);//0
		transformShop.SetRotW(0.0f);//1
		transformShop.SetScale(0.07f);

		transformWoodHouse.SetPosX(-.950f);
		transformWoodHouse.SetPosY(-1.0f);
		transformWoodHouse.SetPosZ(1.52f);
		transformWoodHouse.SetRotX(0.0f);//0
		transformWoodHouse.SetRotY(0.7f);//0
		transformWoodHouse.SetRotZ(0.7f);//0
		transformWoodHouse.SetRotW(0.0f);//1
		transformWoodHouse.SetScale(0.07f);

		// shop
		glm::vec3 min2 = glm::vec3(transformShop.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
		glm::vec3 max2 = glm::vec3(transformShop.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
		PhysicsObject* pOShop = new PhysicsObject(new Aabb(min2, max2), &transformShop);
		PhysicsEngine::GetInstance()->AddObject(*pOShop);
		Warehouse* shop = new Warehouse(transformShop);
		GameObject* landShop = new GameObject(transformShop);
		shop->SetM_Tag("shop");
		map->AddChild((shop)->AddComponent(new RenderModelComponent(RenderEngine::GetInstance()->mShop, transformShop, 180.0f))->AddComponent(new PhysicsObjectComponent(pOShop)));

		// warehouse
		glm::vec3 minWood = glm::vec3(transformWoodHouse.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
		glm::vec3 maxWood = glm::vec3(transformWoodHouse.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
		PhysicsObject* pOShop2 = new PhysicsObject(new Aabb(minWood, maxWood), &transformWoodHouse);
		PhysicsEngine::GetInstance()->AddObject(*pOShop2);
		Warehouse* woodHouse = new Warehouse(transformWoodHouse);
		woodHouse->SetM_Tag("woodHouse");		
		map->AddChild((woodHouse)->AddComponent(new RenderModelComponent(RenderEngine::GetInstance()->mShop, transformWoodHouse, 180.0f))->AddComponent(new PhysicsObjectComponent(pOShop2)));

		if (RenderEngine::GetInstance()->GetFirstLevel() == false) {
			Transform transformWin1;
			Transform transformWin2;
			transformWin1.SetPosX(9.79f);
			transformWin1.SetPosY(-1.0f);
			transformWin1.SetPosZ(10.12f);
			transformWin1.SetRotX(0.0f);//0
			transformWin1.SetRotY(0.7f);//0
			transformWin1.SetRotZ(0.7f);//0
			transformWin1.SetRotW(0.0f);//1
			transformWin1.SetScale(0.07f);

			transformWin2.SetPosX(8.790f);
			transformWin2.SetPosY(-1.0f);
			transformWin2.SetPosZ(10.12f);
			transformWin2.SetRotX(0.0f);//0
			transformWin2.SetRotY(0.7f);//0
			transformWin2.SetRotZ(0.7f);//0
			transformWin2.SetRotW(0.0f);//1
			transformWin2.SetScale(0.07f);

			glm::vec3 min2 = glm::vec3(transformWin1.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
			glm::vec3 max2 = glm::vec3(transformWin1.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
			PhysicsObject* pOWin1 = new PhysicsObject(new Aabb(min2, max2), &transformWin1);
			PhysicsEngine::GetInstance()->AddObject(*pOWin1);
			GameObject* win1 = new GameObject(transformWin1);
			win1->SetM_Tag("win");
			map->AddChild((win1)->AddComponent(new RenderModelComponent(RenderEngine::GetInstance()->mShop, transformWin1, 180.0f))->AddComponent(new PhysicsObjectComponent(pOWin1)));

			glm::vec3 minWood = glm::vec3(transformWin2.GetPos() - glm::vec3(0.4f, 0.1f, 0.4f));
			glm::vec3 maxWood = glm::vec3(transformWin2.GetPos() + glm::vec3(0.4f, 0.1f, 0.4f));
			PhysicsObject* pOWin2 = new PhysicsObject(new Aabb(minWood, maxWood), &transformWin2);
			PhysicsEngine::GetInstance()->AddObject(*pOWin2);
			GameObject* win2 = new GameObject(transformWin2);
			win2->SetM_Tag("win");
			map->AddChild((win2)->AddComponent(new RenderModelComponent(RenderEngine::GetInstance()->mShop, transformWin2, 180.0f))->AddComponent(new PhysicsObjectComponent(pOWin2)));

		}

		map->AddComponent(new Clumsy::RenderInstancedBurnedGroundComponent(RenderEngine::GetInstance()->mBurned, RenderEngine::GetInstance()->groundBurned));
		map->AddComponent(new Clumsy::RenderInstancedGround(RenderEngine::GetInstance()->m3, allTransformsM3));
		map->AddComponent(new Clumsy::RenderInstancedModelComponent(RenderEngine::GetInstance()->m4, allTransformsM4));
		map->AddComponent(new Clumsy::RenderInstancedModelComponent(RenderEngine::GetInstance()->m5, allTransformsM5));
		map->AddComponent(new Clumsy::RenderInstancedModelComponent(RenderEngine::GetInstance()->m6, allTransformsM6));
		map->AddComponent(new Clumsy::RenderInstancedTreesComponent(RenderEngine::GetInstance()->m7, allTransformsM7));
		
    }
}