#pragma once

#include <glm/glm.hpp>

#include "Aabb.h"
#include "Collider.h"
#include "IntersectData.h"

namespace Clumsy
{
	class BoundingSphere : public Collider
	{
	public:
		BoundingSphere(const glm::vec3& center, float radius) :
			Collider(Collider::TYPE_SPHERE),
			m_Center(center), 
			m_Radius(radius) {}

		IntersectData IntersectBoundingSphere(const BoundingSphere& other);
		virtual void Transform(const glm::vec3& translation);

		virtual glm::vec3 GetCenter() const { return m_Center; }

		inline float GetRadius() const { return m_Radius;  }

	private:
		glm::vec3 m_Center;
		float m_Radius;
	};

}
