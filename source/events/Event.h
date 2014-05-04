#pragma once

#include "EpsilonCore.h"
#include "utilities/HashedString.h"

#ifdef _DEBUG
	// Not sure what this is all about but TBB is sad otherwise....
	#define TBB_USE_DEBUG 1
	#define TBB_USE_ASSERT 1
#endif 

#include <tbb/atomic.h>
#include <tbb/mutex.h>
#include <tbb/concurrent_vector.h>

namespace epsilon
{
	class EventDataBase;

	class EventBase
	{
	public:
		friend class EventDataBase;

		EventBase(const HashedString& eventName)
		{
			name = eventName;
		}

		bool operator== (const EventBase& other) const
		{
			return name == other.name;
		}

		const std::size_t GetType() { return name.GetHash(); }

		virtual void ProcessEvents() = 0;

		virtual void Dying() = 0;

	private:
		HashedString name;
	};

	class EventDataBase
	{
	public:
		typedef std::shared_ptr<EventDataBase> Ptr;

		EventDataBase(const std::string eventName)
		{
			name = eventName;
		}
		virtual ~EventDataBase() {}

		bool operator== (const EventDataBase& other) const
		{
			return name == other.name;
		}

		bool operator== (const EventBase& event) const
		{
			return name == event.name;
		}

		const std::size_t& GetType() { return name.GetHash(); }

	protected:
		HashedString name;
	};


	template <typename EventData, typename EventListener>
	class Event : public EventBase
	{
	public:

		Event(const HashedString& eventName) : EventBase(eventName), alive(true)
		virtual ~Event() {}
		
		void Fire(EventData newEvent)
		{
			// Lock the processing mutex (can't add listeners while an event is firing)
			tbb::mutex::scoped_lock processingLock(processingMutex);

			if (alive)
			{
				std::for_each(listeners.begin(), listeners.end(), [&](EventVisitor& listener){
					listener.OnEvent(newEvent);
				});
			}
		}

		void QueueEvent(EventData newEvent)
		{
			if (alive)
			{
				events[currentEvents].push_back(newEvent);
			}
		}
		
		void AddListener(const EventListener& addlistener)
		{
			// Lock or wait for the current event processing to complete before adding a listener
			tbb::mutex::scoped_lock processingLock(processingMutex);

			if (alive)
			{
				listeners.push_back(addlistener);
			}
		}

		void RemoveListener(const EventListener& rmlistener)
		{
			// Lock or wait for the current event processing to complete before removing a listener
			tbb::mutex::scoped_lock processingLock(processingMutex);

			if (alive)
			{
				listeners.erase(std::find_if(listeners.begin(), listeners.end(), [&](EventListener& listener){
					return rmlistener == listener;
				}));
			}
		}

		void ProcessEvents()
		{
			// swap current event queue
			currentEvents = (currentEvents == 0) ? 1 : 0;

			// Send all queued events to the listeners
			std::for_each(events[!currentEvents].begin(), events[!currentEvents].end(), [&](EventData event){
				Fire(event);
			});

			// Erase any events in the event queue
			events.clear();
		}

		void Dying()
		{
			// Block any further event actions
			alive = false;

			// Attempt to get a lock on the processing mutex, 
			// which if there is processing occurring, will block until the 
			// processing has completed
			tbb::mutex::scoped_lock processingLock(processingMutex);
		}

	private:
		tbb::atomic<bool>						alive;
		tbb::mutex								processingMutex;
		tbb::mutex								listenerMutex;
		tbb::concurrent_vector<EventListener&>	listeners;

		// Double buffered event queue so that events can still be queued while processing is occurring
		tbb::atomic<int>						currentEvents;
		tbb::concurrent_vector<EventData>		events[2];
	};
}

