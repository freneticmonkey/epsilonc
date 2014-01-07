#pragma once

#include "EpsilonCore.h"

namespace epsilon
{
	class EventType;
	
	typedef std::hash<std::string> hash_func;
	class EventType
	{
	private:
		struct private_struct {};
	public:
		typedef std::shared_ptr<EventType> Ptr;
		
		static Ptr Create(std::string name);
		
		bool operator==(EventType::Ptr other);
		bool operator==(std::string name);
		bool operator==(const char * name);
		bool operator==(size_t hash);

		size_t hash();

		std::string GetPlainName() { return eventName; }

		EventType(const private_struct &, std::string name);
	private:
		std::string eventName;

		size_t hash_type;
		hash_func toHash;
	};

	class Event :
		public std::enable_shared_from_this<Event>
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<Event> Ptr;

		static Ptr Create(std::string name);

		std::string GetName();
		EventType::Ptr GetType();

		bool IsHandled() { return handled; }
		void SetHandled() { handled = true; }

		//void Send() { EventManager::FireEvent(ThisPtr()); }

		Event(const private_struct &, std::string name);
	private:
		Event::Ptr ThisPtr() { return shared_from_this(); }
		bool handled;
		std::string eventName;
		EventType::Ptr type;
	};
}

