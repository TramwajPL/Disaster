#pragma once

#include <glm/glm.hpp>

#include "Collider.h"
#include "IntersectData.h"
#include "BoundingSphere.h"

namespace Clumsy
{
	// Axis Aligned Bounding Box
	class Aabb : public Collider
	{
	public:
		Aabb(const glm::vec3& minExtends, glm::vec3& maxExtends) :
			Collider(Collider::TYPE_AABB),
			m_MinExtends(minExtends), 
			m_MaxExtends(maxExtends) {}

		IntersectData IntersectAabb(const Aabb& other);
		virtual void Transform(const glm::vec3& translation);

		inline const glm::vec3& GetMinExtends() const { return m_MinExtends; }
		inline const glm::vec3& GetMaxExtends() const { return m_MaxExtends; }

	private:
		glm::vec3 m_MinExtends; // min values vertex
		glm::vec3 m_MaxExtends; // max values vertex 

	};
}