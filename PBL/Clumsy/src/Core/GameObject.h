#pragma once

#include <vector>

#include "Input.h"
#include "Transform.h"

namespace Clumsy 
{
	class EntityComponent;
	class Shader;
	class RenderEngine;
	class PhysicsObjectComponent;
	class RenderModelComponent;
	class TurnClass;

	class GameObject 
	{
	public:
		GameObject(Transform transform);
		GameObject() {}
		~GameObject() {}

		
		Transform GetTransform();
		void SetPos(glm::vec3 vect) { m_Transform.SetPos(vect); }
		glm::vec3 GetPos() { return m_Transform.GetPos(); }

		GameObject* AddComponent(EntityComponent* component);
		void AddChild(GameObject* child);

		std::vector<EntityComponent*> GetComponents();
		std::vector<GameObject*> GetAllChildren();

		
		void Render(Shader& shader);
		void RenderAll(Shader& shader);  

		void Update();
		void UpdateAll();

		std::string GetM_Tag() { return m_Tag; }
		virtual void SetM_Tag(std::string tag) { m_Tag = tag; }	

		virtual bool GetWasCut() { return m_WasCut; }
		virtual void SetWasCut(bool bool1) { }

		virtual void UpdateTurn() {}
		virtual void SetTurnClass(TurnClass* tc) {}
		RenderModelComponent* m_Rmc;
		virtual void SetRenderEnemy(bool renderEnemy) { m_RenderEnemy = renderEnemy; }
		virtual void checkIfRender(int collectedTrees) { } //enemy
		virtual bool GetRenderEnemy() { return m_RenderEnemy; }
		virtual bool GetIsDead() { return isEnemyDead; }
		virtual void SetIsDead(bool dead) { isEnemyDead = dead; }

		void Delete();
		void DeleteAll();
	
	protected:
		int m_CountCollectedTrees;
		bool m_WasCut = false;
		bool m_RenderEnemy = false;
		Transform m_Transform;
		std::string m_Tag = "";
		bool isEnemyDead = false;
	private:
		
		std::vector<glm::vec3> points;
		std::vector<EntityComponent*> m_Components;
		std::vector<GameObject*> m_Children;
	};

}