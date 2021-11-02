#pragma once
#include "../RenderEngine/RenderEngine.h"

namespace Clumsy {

	class DestructionBar
	{
	public:
		DestructionBar(glm::vec3 translate, glm::vec3 color, Shader* shader);

		void Init();

		void Render(glm::vec3 scale, bool destructionBar = false);

		glm::vec3 GetScale() { return m_Scale; }
	private:
		unsigned int VBO, VAO, EBO;
		glm::vec3 m_Scale;
		glm::vec3 m_Translate;
		glm::vec3 m_Color;
		Shader* m_Shader;
	};
}