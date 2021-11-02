
#include "../pch.h"
#include "BoundingSphere.h"

namespace Clumsy
{
	IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other)
	{
		float radiusDistance = m_Radius + other.m_Radius;
		glm::vec3 direction = other.GetCenter() - m_Center;
		float centerDistance = glm::length(direction);
		direction /= centerDistance;

		float distance = centerDistance - radiusDistance;

		return IntersectData(distance < 0, (direction * distance));
	}

	void BoundingSphere::Transform(const glm::vec3& translation)
	{
		m_Center += translation;
	}
}