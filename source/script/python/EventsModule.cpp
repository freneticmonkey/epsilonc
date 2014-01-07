#include "script/python/EventsModule.h"

#include "events/Event.h"
#include "events/EventListener.h"

void initEvents()
{
	using namespace epsilon;

	// make Events package
	object eventsModule( handle<>( borrowed( PyImport_AddModule("epsilon.event") ) ) );
	scope().attr("events") = eventsModule;

	scope eventsScope = eventsModule;

	class_<EventType, EventType::Ptr, boost::noncopyable>("EventType", no_init)
		.def("create", &EventType::Create)
		.staticmethod("create")

		.def_readonly("hash", &EventType::hash)
		.def_readonly("name", &EventType::GetPlainName)

		.def(self == other<EventType::Ptr>())
		.def(self == std::string())
		.def(self == other<const char *>())
		.def(self == size_t())
	;

	class_<Event, Event::Ptr, boost::noncopyable>("Event", no_init)
		.def("create", &Event::Create)
		.staticmethod("create")

		.def_readonly("name", &Event::GetName)
		.def_readonly("type", &Event::GetType)

		.def_readonly("is_handled", &Event::IsHandled)
		.def("set_handled", &Event::SetHandled)
	;
}