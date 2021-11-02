
#include "../pch.h"
#include "PhysicsEngine.h"
#include "BoundingSphere.h"

namespace Clumsy
{
	PhysicsEngine* PhysicsEngine::GetInstance()
	{
		if (!m_Instance)
			m_Instance = new PhysicsEngine;
		return m_Instance;
	}

	void PhysicsEngine::AddObject(const PhysicsObject& object)
	{
		m_Objects.push_back(object);
	}

	void PhysicsEngine::Simulate(float delta)
	{
		for (unsigned int i = 0; i < m_Objects.size(); i++)
		{
			m_Objects[i].Integrate(delta);
		}
	}

	void PhysicsEngine::HandleCollisions()
	{
		for (unsigned int i = 0; i < m_Objects.size(); i++)
		{
			for (unsigned int j = i + 1; j < m_Objects.size(); j++)
			{
				IntersectData intersectData =
					m_Objects[i].GetCollider().Intersect(m_Objects[j].GetCollider());

				if (intersectData.GetDoesIntersect())
				{
					glm::vec3 direction = glm::normalize(intersectData.GetDirection());
					glm::vec3 otherDirection = glm::reflect(direction, glm::normalize(m_Objects[i].GetVelocity()));
					m_Objects[i].SetVelocity(glm::reflect(m_Objects[i].GetVelocity(), otherDirection));
					m_Objects[j].SetVelocity(glm::reflect(m_Objects[j].GetVelocity(), direction));
					std::cout << "collision" << std::endl;
				}
			}
		}
	}
}