
#include "../pch.h"
#include "Aabb.h"
#include "Collider.h"
#include "BoundingSphere.h"

namespace Clumsy
{
	IntersectData Collider::Intersect(const Collider& other) const
	{
		if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_SPHERE)
		{
			BoundingSphere* self = (BoundingSphere*)this;
			return self->IntersectBoundingSphere((BoundingSphere&)other);
		}

		else if (m_Type == TYPE_AABB && other.GetType() == TYPE_AABB)
		{
			Aabb* self = (Aabb*)this;
			return self->IntersectAabb((Aabb&)other);
		}

		else if (m_Type == TYPE_AABB && other.GetType() == TYPE_SPHERE)
		{
			Aabb* self = (Aabb*)this;
			BoundingSphere& otherSelf = (BoundingSphere&)other;

			glm::vec3 pointClosestToSphereCenter;
			pointClosestToSphereCenter.x = glm::max(self->GetMinExtends().x, glm::min(otherSelf.GetCenter().x, self->GetMaxExtends().x));
			pointClosestToSphereCenter.y = glm::max(self->GetMinExtends().y, glm::min(otherSelf.GetCenter().y, self->GetMaxExtends().y));
			pointClosestToSphereCenter.z = glm::max(self->GetMinExtends().z, glm::min(otherSelf.GetCenter().z, self->GetMaxExtends().z));

			glm::vec3 direction = pointClosestToSphereCenter - otherSelf.GetCenter();
			float distanceCenterAndPoint = glm::length(direction);
			direction /= distanceCenterAndPoint;

			float distance = distanceCenterAndPoint - otherSelf.GetRadius();

			return IntersectData(distance < 0, (direction* distance));

		}

		else if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_AABB)
		{
			BoundingSphere* self = (BoundingSphere*)this;
			Aabb& otherSelf = (Aabb&)other;

			glm::vec3 pointClosestToSphereCenter;
			pointClosestToSphereCenter.x = glm::max(otherSelf.GetMinExtends().x, glm::min(self->GetCenter().x, otherSelf.GetMaxExtends().x));
			pointClosestToSphereCenter.y = glm::max(otherSelf.GetMinExtends().y, glm::min(self->GetCenter().y, otherSelf.GetMaxExtends().y));
			pointClosestToSphereCenter.z = glm::max(otherSelf.GetMinExtends().z, glm::min(self->GetCenter().z, otherSelf.GetMaxExtends().z));

			glm::vec3 direction = otherSelf.GetCenter() - pointClosestToSphereCenter;
			float distanceCenterAndPoint = glm::length(direction);
			direction /= distanceCenterAndPoint;

			float distance = distanceCenterAndPoint - self->GetRadius();

			return IntersectData(distance < 0, (direction* distance));
		}

		return IntersectData(false, glm::vec3(0.0f));
	}
}