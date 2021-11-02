#pragma once

#include <glm/glm.hpp>
#include "BoundingSphere.h"

namespace Clumsy
{
	class Plane
	{
	public:
		Plane(const glm::vec3& normal, float distance) :
			m_Normal(normal), 
			m_Distance(distance) {
		}

		Plane Normalized() const;
		IntersectData IntersectSphere(const BoundingSphere& other) const;

		inline const glm::vec3& GetNormal() const { return m_Normal; }
		void SetNormal(glm::vec3 noral) { m_Normal = noral; }
		inline const float GetDistance() const { return m_Distance; }

	private:
		glm::vec3 m_Normal;
		const float m_Distance;
	};
}