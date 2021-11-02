#pragma once

#include "../Core/EntityComponent.h"
#include "../PhysicsEngine/PhysicsEngine.h"

namespace Clumsy 
{
	class PhysicsEngineComponent : public EntityComponent 
	{
	public:
		PhysicsEngineComponent() {}

		virtual void Update() 
		{
			//PhysicsEngine::GetInstance()->HandleCollisions();
		}
	};
}