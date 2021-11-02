#pragma once

#include "../PhysicsEngine/Aabb.h"
#include "../RenderEngine/Shader.h"

namespace Clumsy
{
	class Button
	{
	public:
		Button(glm::vec2 corner, std::string text, glm::vec3 color, glm::vec2 scale);
		~Button() { CleanUp(); }
		void Render(Shader* shader);
		void OnClick();
		void CleanUp();

		std::string GetText() { return m_Text; }
		glm::vec2 GetCorner() { return m_Corner; }
		glm::vec2 GetScale() { return m_Scale; }
		bool IsEnabled() { return m_Enabled; }

		void SetEnabled(bool doopy) { m_Enabled = doopy; }

		void Update(float deltaTime);
		bool m_Ok = false;
		bool m_Fail = false;
		float m_EffectTime = 0;

	private:
		glm::vec2 m_Corner;
		glm::vec2 m_Scale;
		std::string m_Text;
		glm::vec3 m_Color;
		glm::vec3 m_OkColor;
		glm::vec3 m_FailColor;
		unsigned int VBO, VAO;
		bool m_Enabled = false;

	};
}
