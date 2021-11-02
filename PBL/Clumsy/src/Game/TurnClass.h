#pragma once

#include "Player.h"

namespace Clumsy
{
	class GameObject;

	class TurnClass
	{
	public:
		TurnClass(GameObject* go) : 
			m_GameObject(go) {}

		GameObject* GetGameObject() { return m_GameObject; }

		bool isTurn = false;
		bool wasTurnPrev = false;

	private:
		GameObject* m_GameObject;
	};
}