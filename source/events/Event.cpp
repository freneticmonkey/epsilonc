#include "events/Event.h"

namespace epsilon
{
	EventType::Ptr EventType::Create(string name)
	{
		return make_shared<EventType>(private_struct(), name);
	}

	EventType::EventType(const private_struct &, string name)
	{
		eventName = name;
		hash_type = toHash(name);
	}

	bool EventType::operator==(EventType::Ptr other)
	{
		return hash_type == other->hash_type;
	}

	bool EventType::operator==(string name)
	{
		return hash_type == toHash(name);
	}

	bool EventType::operator==(const char * name)
	{
		return hash_type == toHash(string(name));
	}

	bool EventType::operator==(size_t hash)
	{
		return hash_type == hash;
	}

	size_t EventType::hash()
	{
		return hash_type;
	}
	
	Event::Ptr Event::Create(string name)
	{
		return make_shared<Event>(private_struct(), name);
	}

	Event::Event(const private_struct &, string name)
	{
		type = EventType::Create(name);
		eventName = name;
		handled = false;

		// DEBUG INFO
		//printf("Created Event: %s\n", eventName.c_str());
	}

	string Event::GetName()
	{
		return eventName;
	}

	EventType::Ptr Event::GetType()
	{
		return type;
	}
}