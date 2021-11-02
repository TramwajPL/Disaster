#pragma once

#include <glm/glm.hpp>

#include "../Core/GameObject.h"
#include "../PhysicsEngine/Aabb.h"
#include "../RenderEngine/Window.h"
#include "../RenderEngine/Camera.h"
#include "../RenderEngine/RenderEngine.h"
#include "../EventSystem/Listener.h"
#include "../EventSystem/EventSystem.h"
#include "../PhysicsEngine/PhysicsEngine.h"
#include "../Components/PhysicsObjectComponent.h"

namespace Clumsy
{
	class RenderModelComponent;
	class MousePicker : public Listener
	{
	public:
		MousePicker(Camera* camera, Window* window, glm::mat4 projectionMatrix) :
			m_Camera(camera),
			m_Window(window),
			m_ProjectionMatrix(projectionMatrix),
			m_ViewMatrix(camera->GetViewMatrix()) {}

		glm::vec3 GetCurrentRay() { return m_CurrentRay; }
		void Update();
		glm::vec3 CalculateMouseRey();
		glm::vec3 ToWorldCoords(glm::vec4 eyeCoords);
		glm::vec4 ToEyeCoords(glm::vec4 clipCoords);
		glm::vec2 GetNormalisedDeviceCoordinates(float mouseX, float mouseY);

		float CheckCollision(const Collider* aabb);

		glm::vec3 GetPickedObject(glm::vec3 originalPosition);
		void HandleEvent(Event* event);


	private:
		glm::vec3 m_CurrentRay;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		Window* m_Window;
		Camera* m_Camera;
		bool a = true, b = true;
	};
}