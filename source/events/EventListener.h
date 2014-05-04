#pragma once

#include "events/Event.h"

namespace epsilon
{
	template<typename EventData>
	class EventListener
	{
	public:
		typedef std::function<int(EventData)> Callback;

		EventListener(const HashedString& eventName, const Callback& cb)
		{
			name = eventName;
			callback = cb;
		}
		virtual ~EventListener() {}

		bool operator== (const Event& event) const
		{
			return name.GetHash() == event.GetType();
		}

		int OnEvent(const EventData& newEvent)
		{
			return callback(newEvent);
		}

	private:
		HashedString name;

		Callback callback;
	};
}