#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "events/Event.h"

namespace epsilon
{
	using namespace std;

	class EventListener;

	typedef list<EventType::Ptr> EventTypes;

	class EventListener
	{
	public:
		typedef shared_ptr<EventListener> Ptr;
			
		static Ptr Create(EventTypes listenTypes);
		
		EventTypes GetTypes();
		bool ListeningToType(EventType::Ptr type);

		// This should be virtual. So until I think of a better design here.
		// Re-implement this in derived classes
		void Notify(Event::Ptr event);

	private:
		EventListener(EventTypes listenTypes);

		EventTypes types;
	};

	typedef list<EventListener::Ptr> EventListenerList;
	
	typedef map<size_t, EventListenerList> EventListenerMap;
	typedef EventListenerMap::iterator EventListenerIterator;

	typedef list<Event::Ptr> EventList;

	class EventManager
	{
	public:
		static EventManager& getInstance(void)
		{
			static EventManager instance;
			return instance;
		}

		static void AddListener(EventListener::Ptr newListener);
		static void RemoveListener(EventListener::Ptr rmListener);

		static void FireEvent(Event::Ptr event);
		
		static void ProcessEvents(float maxTime);
	private:
		EventManager(void);
		~EventManager(void);

		void AttachListener(EventListener::Ptr newListener);
		void DetachListener(EventListener::Ptr rmListener);

		void QueueEvent(Event::Ptr event);
		void DispatchEvents(float maxTime);

		// Do not implement these functions to prevent access outside 
		// of getInstance
		EventManager(EventManager const&);
		void operator=(EventManager const&);

		EventListenerList listeners;
		EventList events;
	};

}