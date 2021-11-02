#pragma once
#include <glm/glm.hpp>

#include "Shader.h"
#include "../Core/GameObject.h"
#include "../Game/TurnSystem.h"

namespace Clumsy 
{
	class RenderModelComponent;
	class Transform;

	class Cube : public GameObject 
	{
	public:

        Cube(Transform transform);

		void Render(Shader* shaderCube);
		void Update();
		void SetPlayer(RenderModelComponent* rmc);

		bool m_Render = false;

	private:
		RenderModelComponent* m_Player;
		glm::vec3 m_Scale = glm::vec3(0.15f);
		unsigned int cubeVAO, cubeVBO;

	};
}