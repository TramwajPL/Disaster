#pragma once
#include "glfw3.h"

#include "Shader.h"

namespace Clumsy 
{
	class TexturedRect 
	{
	
	public:
		TexturedRect(std::string path, glm::vec3 topRightCorner, glm::vec3 bottomRightCorner, glm::vec3 bottomLeftCorner, glm::vec3 topLeftCorner);
		~TexturedRect();
		void Render(Shader* shader);
		void BackgroundInit(glm::vec3 vector1, glm::vec3 vector2, glm::vec3 vector3, glm::vec3 vector4);

		bool IsEnabled() { return m_Enabled; }
		void SetEnabled(bool doopy) { m_Enabled = doopy; }

	private:
		std::string path;
		unsigned int VBO, VAO, EBO;
		int width, height, nrChannels;
		unsigned int texture;
		bool m_Enabled = true;

	};
}

