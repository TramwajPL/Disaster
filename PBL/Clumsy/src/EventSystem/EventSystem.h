#pragma once

#include <map>
#include <list>
#include <vector>

#include "Listener.h"

namespace Clumsy
{
	class EventSystem
	{
	private:
		static EventSystem* m_Instance;

		std::map<EVENT_ID, std::vector<Listener*>> m_Observers;		
		std::list<Event> m_CurrentEvents; 

		EventSystem() {}
		~EventSystem() { ShutDown(); }
		EventSystem(const EventSystem& rhs) {}
		EventSystem& operator=(const EventSystem& rhs) {}

		// Helper methods
		bool AlreadySubscribed(EVENT_ID eventId, Listener* listener);
		void DispatchEvent(Event* event);

	public:
		static EventSystem* GetInstance();

		void SubscribeListener(EVENT_ID eventId, Listener* listener);
		void UnsubscribeListener(EVENT_ID eventId, Listener* listener);
		void UnsubscribeAll(Listener* listener); // unsubscribe a listener from all events
		void SendEvent(EVENT_ID eventId, void* data = 0);
		void ProcessEvents();
		void ClearEvents();
		void ShutDown();
	};
}