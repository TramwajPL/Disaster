#pragma once

#include <glm/glm.hpp>

#include "BoundingSphere.h"
#include "../Core/Transform.h"

namespace Clumsy
{
	class PhysicsObject
	{
	public:
		PhysicsObject(Collider* collider, const glm::vec3& velocity, Transform* transform) :
			m_Position(transform->GetPos()), 
			m_OldPosition(transform->GetPos()),
			m_Velocity(velocity), 
			m_Collider(collider) {}
		
		PhysicsObject(Collider* collider, Transform* transform) :
			m_Position(transform->GetPos()),
			m_OldPosition(transform->GetPos()),
			m_Collider(collider) {}

		PhysicsObject(const PhysicsObject& other);
		void operator=(PhysicsObject other);
		virtual ~PhysicsObject();

		void Integrate(float delta);

		inline  glm::vec3& GetPosition()  { return m_Position; }
		inline const glm::vec3& GetVelocity() const { return m_Velocity; }

		inline const Collider& GetCollider()
		{
			glm::vec3 translation = m_Position - m_OldPosition;
			m_OldPosition = m_Position;
			m_Collider->Transform(translation);

			return *m_Collider;
		}

		inline void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_OldPosition;
		glm::vec3 m_Velocity;

		Collider* m_Collider;
	};
}
