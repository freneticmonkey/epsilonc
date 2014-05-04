#include "events/EventManager.h"

namespace epsilon
{
	EventManager::EventManager(void)
	{
		Log("EventManager initialised");
	}

	EventManager::~EventManager(void)
	{
		// Clean up any remaining registered events
		std::for_each(events.begin(), events.end(), [&](std::pair<std::size_t, EventBase *> event){
			DeregisterEvent(event.first);
		});
	}
	
	void EventManager::ProcessEvents(float maxTime)
	{
		EventManager::GetInstance().DispatchEvents(maxTime);
	}

	void EventManager::RegisterEvent(EventBase * newEvent)
	{
		EventBase * found = FindEvent(newEvent->GetType());

		if (found == nullptr)
		{
			events[newEvent->GetType()] = newEvent;
		}
	}

	void EventManager::DeregisterEvent(const std::size_t& eventId)
	{
		EventBase * foundEvent = FindEvent(eventId);

		if (foundEvent != nullptr)
		{
			// Notifiy the event that it's going to die
			// which will prevent any further actions and wait 
			// until any active processing is complete
			foundEvent->Dying();

			// Remove it from the map
			events.erase(eventId);

			// Delete it.
			delete foundEvent;
		}
	}

	EventBase * EventManager::FindEvent(const std::size_t& eventId)
	{
		EventBase * foundEvent = nullptr;

		EventMap::iterator found = events.find(eventId);

		if (found != events.end())
		{
			foundEvent = events[eventId];
		}
		
		return foundEvent;
	}
	
	void EventManager::DispatchEvents(float maxTime)
	{
		// Implement time limiter here using maxTime
		clock.restart();

		for (EventMap::iterator e = events.begin(); e != events.end(); e++)
		{
			// Process the queue events for this event type.
			(*e).second->ProcessEvents();

			// If our time is up.
			if (clock.getElapsedTime().asMilliseconds() >= maxTime)
			{
				break;
			}
		}
	}
}