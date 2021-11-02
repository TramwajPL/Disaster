#pragma once

#include <glm/glm.hpp>

namespace Clumsy
{
	class IntersectData
	{
	public:
		IntersectData(const bool doesIntersect, const glm::vec3& direction) :
			m_DoesIntersect(doesIntersect), 
			m_Direction(direction) {}

		inline bool GetDoesIntersect() const { return m_DoesIntersect;  }
		inline float GetDistance() const { return glm::length(m_Direction); }
		inline const glm::vec3 GetDirection() const { return m_Direction; }

	private:
		const bool m_DoesIntersect;
		const glm::vec3 m_Direction;
	};
}