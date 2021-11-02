
#include "../pch.h"

#include "EventSystem.h"

namespace Clumsy
{
	EventSystem* EventSystem::GetInstance()
	{
		if (!m_Instance)
			m_Instance = new EventSystem;
		return m_Instance;
	}

	bool EventSystem::AlreadySubscribed(EVENT_ID eventId, Listener* listener)
	{
		bool alreadySubbed = false;

		std::map<EVENT_ID, std::vector<Listener*>>::iterator it = m_Observers.find(eventId);
		if (it != m_Observers.end())
		{
			for (int i = 0; i < m_Observers[eventId].size(); i++)
			{
				if (m_Observers[eventId][i] == listener)
				{
					alreadySubbed = true;
					break;
				}
			}
		}
		return alreadySubbed;
	}

	void EventSystem::DispatchEvent(Event* event)
	{
		for (int i = 0; i < m_Observers[event->GetEventId()].size(); i++)
		{
			m_Observers[event->GetEventId()][i]->HandleEvent(event);
		}
	}

	void EventSystem::SubscribeListener(EVENT_ID eventId, Listener* listener)
	{
		if (!listener || AlreadySubscribed(eventId, listener))
			return;

		m_Observers[eventId].push_back(listener);
	}

	void EventSystem::UnsubscribeListener(EVENT_ID eventId, Listener* listener) 
	{
		std::vector<Listener*>::iterator iter = std::find(m_Observers[eventId].begin(), m_Observers[eventId].end(), listener);

		if(iter != m_Observers[eventId].end())
			m_Observers[eventId].erase(iter);
	}

	void EventSystem::UnsubscribeAll(Listener* listener)
	{
		for (std::map<EVENT_ID, std::vector<Listener*> >::iterator it = m_Observers.begin(); it != m_Observers.end(); ++it)
		{
			for (std::vector<Listener*>::iterator b = (*it).second.begin(); b != (*it).second.end(); ++b)
			{
				if (*b == listener)
				{
					b = it->second.erase(b);
				}
			}
		}
	} 

	void EventSystem::SendEvent(EVENT_ID eventId, void* data)
	{
		Event event(eventId, data);
		m_CurrentEvents.push_back(event);
	}

	void EventSystem::ProcessEvents()
	{
		while (m_CurrentEvents.size())
		{
			DispatchEvent(&m_CurrentEvents.front());
			m_CurrentEvents.pop_front();
		}
	}

	void EventSystem::ClearEvents()
	{
		m_CurrentEvents.clear();
	}

	void EventSystem::ShutDown()
	{
		m_Observers.clear();
		m_CurrentEvents.clear();
	}
}