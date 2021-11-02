#pragma once

#include "../Core/EntityComponent.h"
#include "../PhysicsEngine/PhysicsObject.h"

namespace Clumsy 
{

	class PhysicsObjectComponent : public EntityComponent 
	{
	public:
		PhysicsObjectComponent(PhysicsObject* physicsObject) : m_PhysicsObject(physicsObject) {}
		~PhysicsObjectComponent() {}

		virtual void Update() {}
	
		Collider getCollider() 
		{
			return m_PhysicsObject->GetCollider();
		}

	private:
		PhysicsObject* m_PhysicsObject;

	};
}