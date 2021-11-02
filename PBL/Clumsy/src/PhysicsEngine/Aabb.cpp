
#include "../pch.h"

#include "Aabb.h"

namespace Clumsy
{
	IntersectData Aabb::IntersectAabb(const Aabb& other)
	{
		glm::vec3 distances1 = other.GetMinExtends() - m_MaxExtends;
		glm::vec3 distances2 = m_MinExtends - other.GetMaxExtends();
		glm::vec3 distances = glm::vec3(glm::max(distances1.x, distances2.x), glm::max(distances1.y, distances2.y), glm::max(distances1.z, distances2.z));

		float maxDistance = glm::max(glm::max(distances.x, distances.y), distances.z);

		return IntersectData(maxDistance < 0, distances);
	}

	void Aabb::Transform(const glm::vec3& translation)
	{
		m_MinExtends += translation;
		m_MaxExtends += translation;
	}
}