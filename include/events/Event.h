#pragma once

#include "EpsilonCore.h"

namespace epsilon
{
	using namespace std;
	class EventType;

	typedef hash<string> hash_func;
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

	class Event
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<Event> Ptr;

		static Ptr Create(string name);

		const char * GetName();
		EventType::Ptr GetType();

		bool IsHandled() { return handled; }
		void SetHandled() { handled = true; }

		Event(const private_struct &, string name);
	private:

		bool handled;
		string eventName;
		EventType::Ptr type;
	};
}

