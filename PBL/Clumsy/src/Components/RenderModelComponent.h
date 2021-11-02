#pragma once

#include <glm/gtx/string_cast.hpp>

#include "../RenderEngine/Model.h"
#include "../Core/EntityComponent.h"

namespace Clumsy {

	class RenderModelComponent : public EntityComponent
	{
	public:
		RenderModelComponent(Model* model, Transform transform, float angle, bool isEnemy = false) :
			m_Model(model), m_Transform(transform), m_Angle(angle), m_IsEnemy(isEnemy)	{}

		virtual void Render(Shader& shader) 

		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, m_Transform.GetPos());
			glm::vec3 vec;
			vec.x = m_Transform.GetRot()->x;
			vec.y = m_Transform.GetRot()->y;
			vec.z = m_Transform.GetRot()->z;
			model = glm::rotate(model, glm::radians(m_Angle), vec);
			/*if (m_IsEnemy == true) //remove
			{
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f));
			}*/
			model = glm::scale(model, glm::vec3(m_Transform.GetScale()));

			shader.setMat4("model", model);

			if (m_Model->hasBones) {
				m_Model->Draw(shader);
			}
			else
				m_Model->Draw2(shader);
			
		}

		void SetTransform(glm::vec3 tr)
		{
			m_Transform.SetPos(tr);
		}

		virtual void Update() 
		{
			SetTransform(m_Transform.GetPos());
		}

		Transform m_Transform;
		Model* m_Model;
		void SetAngle(float newAngle) { m_Angle = newAngle; }
		float GetAngle() { return m_Angle; }

	private:
		float m_Angle;
		bool m_IsEnemy;
	};
}