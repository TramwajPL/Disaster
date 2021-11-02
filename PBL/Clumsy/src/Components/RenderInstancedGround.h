#pragma once

#include <glad\glad.h>
#include <glm/gtx/string_cast.hpp>

#include "../RenderEngine/Model.h"
#include "../Core/EntityComponent.h"

namespace Clumsy {

	class RenderInstancedGround : public EntityComponent
	{
	public:
		RenderInstancedGround(Model* model, std::vector<Transform> transform) : m_Model(model), m_Transform(transform)
		{

			amount = m_Transform.size();
			glm::mat4* modelMatrices;
			modelMatrices = new glm::mat4[amount];

			for (unsigned int i = 0; i < amount; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);

				model = glm::translate(model, m_Transform[i].GetPos());

				glm::vec3 vec;
				vec.x = m_Transform[i].GetRot()->x;
				vec.y = m_Transform[i].GetRot()->y;
				vec.z = m_Transform[i].GetRot()->z;
				model = glm::rotate(model, glm::radians(90.0f), vec);

				model = glm::scale(model, glm::vec3(m_Transform[i].GetScale()));

				// 4. now add to list of matrices
				modelMatrices[i] = model;
			}

			// configure instanced array
			// -------------------------
			unsigned int buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

			// set transformation matrices as an instance vertex attribute (with divisor 1)
			// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
			// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
			// -----------------------------------------------------------------------------------------------------------------------------------
			for (unsigned int i = 0; i < m_Model->meshes.size(); i++)
			{
				unsigned int VAO = m_Model->meshes[i].VAO;
				glBindVertexArray(VAO);
				// set attribute pointers for matrix (4 times vec4)
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				glEnableVertexAttribArray(7);
				glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				glEnableVertexAttribArray(8);
				glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

				glVertexAttribDivisor(5, 1);
				glVertexAttribDivisor(6, 1);
				glVertexAttribDivisor(7, 1);
				glVertexAttribDivisor(8, 1);

				glBindVertexArray(0);
			}

		}
		void Update(std::vector<Transform> transform2) {
			amount = transform2.size();
			glm::mat4* modelMatrices;
			modelMatrices = new glm::mat4[amount];

			for (unsigned int i = 0; i < amount; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);

				model = glm::translate(model, transform2[i].GetPos());

				glm::vec3 vec;
				vec.x = transform2[i].GetRot()->x;
				vec.y = transform2[i].GetRot()->y;
				vec.z = transform2[i].GetRot()->z;
				model = glm::rotate(model, glm::radians(90.0f), vec);

				model = glm::scale(model, glm::vec3(transform2[i].GetScale()));

				// 4. now add to list of matrices
				modelMatrices[i] = model;
			}

			// configure instanced array
			// -------------------------
			unsigned int buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

			// set transformation matrices as an instance vertex attribute (with divisor 1)
			// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
			// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
			// -----------------------------------------------------------------------------------------------------------------------------------
			for (unsigned int i = 0; i < m_Model->meshes.size(); i++)
			{
				unsigned int VAO = m_Model->meshes[i].VAO;
				glBindVertexArray(VAO);
				// set attribute pointers for matrix (4 times vec4)
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				glEnableVertexAttribArray(7);
				glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				glEnableVertexAttribArray(8);
				glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

				glVertexAttribDivisor(5, 1);
				glVertexAttribDivisor(6, 1);
				glVertexAttribDivisor(7, 1);
				glVertexAttribDivisor(8, 1);

				glBindVertexArray(0);
			}
		}

		virtual void Render(Shader& shader)
		{
			Update(RenderEngine::GetInstance()->ground);
			m_Model->Draw3(shader, amount);
		}


	private:
		std::vector<Transform> m_Transform;
		Model* m_Model;
		unsigned int amount;
	};
}