#pragma once

#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Core/Time.h"
#include "../Core/Timestep.h"
#include "../RenderEngine/Window.h"

namespace Clumsy {

	class Transform
	{
	public:
		Transform(const glm::vec3& pos = glm::vec3(0, 0, 0), const glm::quat& rot = glm::quat(0.0f, 0.0f, 0.0f, 1.0f), float scale = 1.0f) :
			m_pos(pos),
			m_rot(rot),
			m_scale(scale),
			m_parent(0),
			m_parentMatrix(glm::mat4(1.0f)) {}

		glm::mat4 GetTransformation() const;
		bool HasChanged();
		void Update();
		void Rotate(const glm::vec3& axis, float angle);
		void Rotate(const glm::quat& rotation);
		void LookAt(const glm::vec3& point, const glm::vec3& up);

		inline glm::vec3 GetPos() { return m_pos; }
		inline const glm::vec3& GetPos() const { return m_pos; }
		inline glm::quat* GetRot() { return &m_rot; }
		inline const glm::quat& GetRot() const { return m_rot; }
		inline float GetScale() const { return m_scale; }
		inline glm::vec3 GetTransformedPos(glm::vec3 newVector) const;
		glm::vec3 GetTransformedPos();
		glm::quat GetTransformedRot() const;

		inline void SetPos(glm::vec3& pos) { m_pos = pos; }

		glm::vec3 lerp(glm::vec3 currentPos, glm::vec3 goalPos, float delta);

		inline void SetPosX(const float x) { m_pos.x = x; }
		inline void SetPosY(const float y) { m_pos.y = y; }
		inline void SetPosZ(const float z) { m_pos.z = z; }
		inline void SetRot(const glm::quat& rot) { m_rot = rot; }
		inline void SetRotX(const float x) { m_rot.x = x; }
		inline void SetRotY(const float y) { m_rot.y = y; }
		inline void SetRotZ(const float z) { m_rot.z = z; }
		inline void SetRotW(const float w) { m_rot.z = w; }
		inline void SetScale(float scale) { m_scale = scale; }
		inline void SetParent(Transform* parent) { m_parent = parent; }

		glm::mat4 SetRotation();

	private:
		const glm::mat4& GetParentMatrix() const;
		float m_LastFrameTime = 0.0f;

		glm::vec3 m_pos;
		glm::quat m_rot;
		float m_scale;
		Transform* m_parent;
		mutable glm::mat4 m_parentMatrix;
		mutable glm::vec3 m_oldPos;
		mutable glm::quat m_oldRot;
		mutable float m_oldScale;
		mutable bool m_initializedOldStuff;
	};
}