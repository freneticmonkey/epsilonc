#pragma once

#include "events/Event.h"

namespace epsilon
{
typedef std::list<EventType::Ptr> EventTypes;

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

	typedef std::list<EventListener::Ptr> EventListenerList;

}