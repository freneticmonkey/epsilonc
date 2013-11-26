#include "events/EventManager.h"

namespace epsilon
{
	EventManager::EventManager(void)
	{
		Log("EventManager initialised");
	}

	EventManager::~EventManager(void)
	{
	}

	void EventManager::AddListener(EventListener::Ptr newListener)
	{
		EventManager::GetInstance().AttachListener(newListener);
	}

	void EventManager::RemoveListener(EventListener::Ptr rmListener)
	{
		EventManager::GetInstance().DetachListener(rmListener);
	}

	void EventManager::AttachListener(EventListener::Ptr newListener)
	{
		listeners.push_back(newListener);
		
		// DEBUG INFO
		//string types;
		//EventTypes ets = newListener->getTypes();
		//for ( EventTypes::iterator t = ets.begin(); t != ets.end(); t++ )
		//{
		//	types += string(",") + (*t)->getPlainName();
		//}
		//printf("Adding listener for: (%s)\n", types.c_str());
	}

	void EventManager::DetachListener(EventListener::Ptr rmListener)
	{
		listeners.remove(rmListener);
	}

	void EventManager::FireEvent(Event::Ptr event)
	{
		EventManager::GetInstance().QueueEvent(event);
	}

	void EventManager::ProcessEvents(float maxTime)
	{
		EventManager::GetInstance().DispatchEvents(maxTime);
	}
	
	void EventManager::QueueEvent(Event::Ptr event)
	{
		events.push_back(event);
	}

	void EventManager::DispatchEvents(float maxTime)
	{
		// Implement time limiter here using maxTime
		EventListener::Ptr listener;
		Event::Ptr event;

		for (EventList::iterator e = events.begin(); e != events.end(); e++ )
		{
			event = (*e);
			for (EventListenerList::iterator l = listeners.begin(); l != listeners.end(); l++ )
			{
				listener = (*l);

				if ( listener->ListeningToType( event->GetType() ) )
				{
					listener->Notify(event);

					if ( event->IsHandled() )
					{
						break;
					}
				}
			}
		}

		// Clear all events
		events.clear();

	}
}