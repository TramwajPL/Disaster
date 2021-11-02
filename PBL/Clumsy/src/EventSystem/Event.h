#pragma once

#include <string>
#define EVENT_ID std::string

namespace Clumsy
{
    class Event
    {
    public:
        Event(EVENT_ID eventId, void* parameter = 0) :
            m_EventId(eventId), 
			m_Parameter(parameter){}
        ~Event() {}
        EVENT_ID GetEventId() { return m_EventId; }
		inline void* GetParameter() { return m_Parameter; }

    private:
        EVENT_ID m_EventId;
		void* m_Parameter;
    };

}
