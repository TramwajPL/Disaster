#pragma once

#include "Input.h"
#include "GameObject.h"
#include "../GUI/MenuGUI.h"
#include "../GUI/StoreGUI.h"
#include "../GUI/CreditsGUI.h"
#include "../EventSystem/Listener.h"
#include "../RenderEngine/RenderEngine.h"
#include "../RenderEngine/PostProcessor.h"
#include "../PhysicsEngine/PhysicsEngine.h"


namespace Clumsy 
{
	class Model;

	class Game : public Listener
	{
	public:
		virtual void Init() {} 
		virtual void Move() {}
		void Render();
		void Update(float deltaTime);
		void AddToScene(GameObject* child);

		void HandleEvent(Event* event);

		GameObject getRoot() { return m_Root; }
		void SceneParser(GameObject* map, std::string filename);
		std::vector<Transform> allTransformsM7;

	private:
		GameObject m_Root;
		bool mainMenuControler = true;
	};
}