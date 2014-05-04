#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "events/Event.h"

#include <SFML/System/Clock.hpp>

namespace epsilon
{
	typedef std::map<std::size_t, EventBase *> EventMap;

	class EventManager
	{
	public:
		static EventManager& GetInstance(void)
		{
			static EventManager instance;
			return instance;
		}		
		~EventManager(void);

		static void ProcessEvents(float maxTime);

		void RegisterEvent(EventBase * newEvent);
		void DeregisterEvent(const std::size_t& eventId);

		EventBase * FindEvent(const std::size_t& eventId);

	private:
		EventManager(void);

		void DispatchEvents(float maxTime);

		EventManager(EventManager const&);

		EventMap events;

		sf::Clock clock;
	};

}