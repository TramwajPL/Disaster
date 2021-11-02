
#include "../pch.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include "Button.h"

namespace Clumsy
{

	Button::Button(glm::vec2 corner, std::string text, glm::vec3 color, glm::vec2 scale) :
		m_Corner(corner),
		m_Text(text),
		m_Color(color),
		m_Scale(scale)
	{
		float vertices[] = { 
			0.5f, 0.5f, 0.0f,  // top right 
			-0.5f, 0.5f, 0.0f, // bottom right 
			-0.5f, -0.5f, 0.0f, // bottom left 
			-0.5f, -0.5f, 0.0f, // bottom left 
			0.5f, -0.5f, 0.0f,  // top left //really: bootm right
			0.5f, 0.5f, 0.0f, // top right 
		};

		m_OkColor = glm::vec3(0.2f, 0.72f, 0.16f);
		m_FailColor = glm::vec3(0.8f, 0.06f, 0.06f);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);		
	}

	void Button::Render(Shader* shader)
	{
		shader->use();
		glBindVertexArray(VAO); 
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(m_Corner, 0.0f));
		model = glm::scale(model, glm::vec3(m_Scale, 0.0f));
		shader->setMat4("model",model);
		if (m_Ok)
		{
			shader->setVec3("Color", m_OkColor);
		}
		else if (m_Fail)
		{
			shader->setVec3("Color", m_FailColor);
		}
		else
		{
			shader->setVec3("Color", m_Color);
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Button::OnClick()
	{
		std::cout << m_Text + " Button clicked " << std::endl;
	}
	
	void Button::CleanUp()
	{
		glDeleteVertexArrays(1, &VAO); 
		glDeleteBuffers(1, &VBO); 
	}

	void Button::Update(float deltaTime)
	{
		if (m_EffectTime > 0.0f)
		{
			m_EffectTime -= deltaTime;
			if (m_EffectTime <= 0.0f)
			{
				m_Ok = false;
				m_Fail = false;
			}
		}
	}
}