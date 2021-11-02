#pragma once
#include "Shader.h"
#include "../Core/EntityComponent.h"

namespace Clumsy {

	class CoreEngine;

	class BaseLight : public EntityComponent
	{
	public:
		BaseLight(glm::vec3 color, float intensity, Shader shader) : m_Color(color), m_Intensity(intensity), m_Shader(shader) {}

		glm::vec3 GetColor() { return m_Color; }
		float GetIntensity() { return m_Intensity; }

		virtual void AddToEngine(CoreEngine* engine) const;
		inline const Shader& GetShader()         const { return m_Shader; }

		void SetColor(glm::vec3 newColor) { m_Color = newColor; }
		void SetIntensity(float newIntensity) { m_Intensity = newIntensity; }
		void SetShader(Shader newShader) { m_Shader = newShader; }

	protected:
		glm::vec3 m_Color;
		float m_Intensity;
		Shader m_Shader;
	};

	class DirectionalLight  : public BaseLight
	{
	public:
		DirectionalLight(glm::vec3 direction, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), float intensity = 0)
			: BaseLight(color, intensity, Shader("../Clumsy/src/Shaders/phongVS.glsl", "../Clumsy/src/Shaders/phongFS.glsl")), m_Direction(direction) {}

		glm::vec3 GetDirection() { return m_Direction; }
		void SetDirection(glm::vec3 newDirection) { m_Direction = newDirection; }

	private:
		glm::vec3 m_Direction;
	};
}