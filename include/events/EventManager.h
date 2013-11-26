#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "events/Event.h"
#include "events/EventListener.h"

namespace epsilon
{
	typedef std::map<size_t, EventListenerList> EventListenerMap;
	typedef EventListenerMap::iterator EventListenerIterator;

	typedef std::list<Event::Ptr> EventList;

	class EventManager
	{
	public:
		static EventManager& GetInstance(void)
		{
			static EventManager instance;
			return instance;
		}		
		~EventManager(void);

		static void AddListener(EventListener::Ptr newListener);
		static void RemoveListener(EventListener::Ptr rmListener);

		static void FireEvent(Event::Ptr event);
		
		static void ProcessEvents(float maxTime);
	private:
		EventManager(void);

		void AttachListener(EventListener::Ptr newListener);
		void DetachListener(EventListener::Ptr rmListener);

		void QueueEvent(Event::Ptr event);
		void DispatchEvents(float maxTime);

		// Do not implement these functions to prevent access outside 
		// of GetInstance
		EventManager(EventManager const&);
		void operator=(EventManager const&);

		EventListenerList listeners;
		EventList events;
	};

}