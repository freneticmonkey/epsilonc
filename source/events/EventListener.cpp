
#include "events/EventListener.h"

namespace epsilon
{
	EventListener::Ptr EventListener::Create(EventTypes listenTypes)
	{
		return EventListener::Ptr(new EventListener(listenTypes));
	}

	EventListener::EventListener(EventTypes listenTypes)
	{
		types = listenTypes;
	}

	EventTypes EventListener::GetTypes()
	{
		return types;
	}

	bool EventListener::ListeningToType(EventType::Ptr type)
	{
		EventType::Ptr listenType;

		for (EventTypes::iterator et = types.begin(); et != types.end(); et++ )
		{
			listenType = (*et);
			if ( (*listenType) == type )
			{
				return true;
			}
		}
		return false;
	}

	void EventListener::Notify(Event::Ptr event)
	{
		// DEBUG INFO:
		//printf("Received Event: %s\n", event->getName());
	}
}