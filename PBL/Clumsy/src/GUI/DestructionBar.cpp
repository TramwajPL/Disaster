#include "../pch.h"
#include "DestructionBar.h"

namespace Clumsy 
{
	DestructionBar::DestructionBar(glm::vec3 translate, glm::vec3 color, Shader* shader) :
		m_Translate(translate), m_Color(color), m_Shader(shader)
	{
		Init();
	}

	void DestructionBar::Init()
	{
		float vertices[] = {
			 1.0f,  1.0f, 0.0f,  // top right
			 1.0f, 0.0f, 0.0f,  // bottom right
			 0.0f, 0.0f, 0.0f,  // bottom left
			 0.0f,  1.0f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DestructionBar::Render(glm::vec3 scale, bool destructionBar)
	{
		m_Scale = scale;
		m_Shader->use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_Translate);
		model = glm::scale(model, scale);
		m_Shader->setMat4("model", model);
		m_Shader->setVec3("Color", m_Color);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}