#pragma once

#include <glm/glm.hpp>

#include "IntersectData.h"
#include "ReferenceCounter.h"

namespace Clumsy
{
	class Collider : public ReferenceCounter
	{
	public:
		enum
		{
			TYPE_SPHERE,
			TYPE_AABB,

			TYPE_SIZE
		};

		Collider(int type) : 
			ReferenceCounter(),
			m_Type(type) {}

		IntersectData Intersect(const Collider& other) const;
		virtual void Transform(const glm::vec3& translation) {}
		virtual glm::vec3 GetCenter() const { return glm::vec3(0, 0, 0); }

		virtual const glm::vec3& GetMinExtends() const { return glm::vec3(0, 0, 0); }
		virtual const glm::vec3& GetMaxExtends() const { return glm::vec3(0, 0, 0); }

		inline int GetType() const { return m_Type; }

	private:
		int m_Type;
	};
}
