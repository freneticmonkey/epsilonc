#pragma once

#include "EpsilonCore.h"

namespace epsilon
{
	class EventType;
	
	typedef std::hash<string> hash_func;
	class EventType
	{
	private:
		struct private_struct {};
	public:
		typedef shared_ptr<EventType> Ptr;
		
		static Ptr Create(string name);
		
		bool operator==(EventType::Ptr other);
		bool operator==(string name);
		bool operator==(const char * name);
		bool operator==(size_t hash);

		size_t hash();

		string GetPlainName() { return eventName; }

		EventType(const private_struct &, string name);
	private:
		string eventName;

		size_t hash_type;
		hash_func toHash;
	};

	class Event :
		public enable_shared_from_this<Event>
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<Event> Ptr;

		static Ptr Create(string name);

		string GetName();
		EventType::Ptr GetType();

		bool IsHandled() { return handled; }
		void SetHandled() { handled = true; }

		//void Send() { EventManager::FireEvent(ThisPtr()); }

		Event(const private_struct &, string name);
	private:
		Event::Ptr ThisPtr() { return shared_from_this(); }
		bool handled;
		string eventName;
		EventType::Ptr type;
	};
}

