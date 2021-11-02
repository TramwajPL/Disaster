
#include "../pch.h"
#include "Transform.h"

namespace Clumsy{

	bool Transform::HasChanged()
	{
		if (m_parent != 0 && m_parent->HasChanged())
		{
			return true;
		}

		if (m_pos != m_oldPos)
		{
			return true;
		}

		if (m_rot != m_oldRot)
		{
			return true;
		}

		if (m_scale != m_scale)
		{
			return true;
		}

		return false;
	}

	void Transform::Update()
	{
		if (m_initializedOldStuff)
		{
			m_oldPos = m_pos;
			m_oldRot = m_rot;
			m_oldScale = m_scale;
		}
		else
		{
			m_oldPos = m_pos + glm::vec3(1, 1, 1);
			m_oldRot = m_rot * 0.5f;
			m_oldScale = m_scale + 1;
			m_initializedOldStuff = true;
		}
	}

	void Transform::Rotate(const glm::vec3& axis, float angle)
	{
		Rotate(glm::quat(axis.x, axis.y, axis.z, angle));
	}

	void Transform::Rotate(const glm::quat& rotation)
	{
		m_rot = glm::normalize(glm::quat((rotation * m_rot)));
	}

	glm::mat4 Transform::GetTransformation() const
	{
		glm::mat4 translationMatrix = glm::mat4(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		glm::vec3 scaleScale(m_scale, m_scale, m_scale);

		translationMatrix = glm::translate(translationMatrix, m_pos);
		rotationMatrix = glm::rotate(rotationMatrix, m_rot.x, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, m_rot.y, glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		scaleMatrix = glm::scale(scaleMatrix, scaleScale);

		glm::mat4 result = translationMatrix * rotationMatrix * scaleMatrix;

		return GetParentMatrix() * result;
	}

	const glm::mat4& Transform::GetParentMatrix() const
	{
		if (m_parent != 0 && m_parent->HasChanged())
		{
			m_parentMatrix = m_parent->GetTransformation();
		}

		return m_parentMatrix;
	}

	inline glm::vec3 Transform::GetTransformedPos(glm::vec3 newVector) const
	{
		glm::vec4 pos = glm::vec4(newVector.x, newVector.y, newVector.z, 1.0f);
		pos = GetTransformation() * pos;
		glm::vec3 pos2 = glm::vec3(pos.x, pos.y, pos.z);
		return pos2;
	}

	glm::vec3 Transform::GetTransformedPos()
	{
		glm::vec4 pos = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		pos = GetTransformation() * pos;
		glm::vec3 pos2 = glm::vec3(pos.x, pos.y, pos.z);
		return pos2;
	}

	glm::quat Transform::GetTransformedRot() const
	{
		glm::quat parentRot = glm::quat(0, 0, 0, 1);

		if (m_parent)
		{
			parentRot = m_parent->GetTransformedRot();
		}

		return parentRot * m_rot;
	}

	glm::vec3 Transform::lerp(glm::vec3 currentPos, glm::vec3 goalPos, float delta)
	{
		return currentPos + delta*(goalPos - currentPos);
	}

	glm::mat4 Transform::SetRotation()
	{
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, m_rot.x, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, m_rot.y, glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

		return rotationMatrix;
	}
}