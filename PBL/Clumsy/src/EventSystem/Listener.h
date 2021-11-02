#pragma once

#include "Event.h"

namespace Clumsy
{
	class Listener
	{
	public:
		Listener() {}
		virtual ~Listener() {}
		virtual void HandleEvent(Event* event) = 0;
	};
}
