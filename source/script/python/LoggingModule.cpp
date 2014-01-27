#include "script/python/LoggingModule.h"

#include "logging/Logging.h"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	LoggerLog, Logger::Log, 1, 2
)

void initLogging()
{
	//using namespace epsilon;

	// make Logging package
	object loggingModule( handle<>( borrowed( PyImport_AddModule("epsilon.logging") ) ) );
	scope().attr("logging") = loggingModule;

	scope loggingScope = loggingModule;

	// This is a virtual base class
	class_<epsilon::LogListener, epsilon::LogListener::Ptr, boost::noncopyable>("LogListener", no_init);

	void (*LogNamedContent)(std::string, std::string)= &epsilon::Logger::Log;

	class_<epsilon::Logger, boost::noncopyable>("Logger", no_init)
		//.def("log", (void (Logger::*)(string, string) ) &Logger::Log, LoggerLog())
		.def("log", LogNamedContent)
		.staticmethod("log")

		.def("add_listener", &epsilon::Logger::addListener)
		.def("remove_listener", &epsilon::Logger::removeListener)
	;
}