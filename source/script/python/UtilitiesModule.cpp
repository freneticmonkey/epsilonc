#include "script/python/UtilitiesModule.h"
#include "script/PythonLogListener.h"

void initUtilities()
{

	using namespace epsilon;

	// make math package
	object utilityModule( handle<>( borrowed( PyImport_AddModule("epsilon.util") ) ) );
	scope().attr("util") = utilityModule;
	scope utilScope = utilityModule;

	class_<PythonLogListener>("PythonLogListener", init<>())
		.def(init<std::string>())
		.def("write", &PythonLogListener::OutputLog)
	;
}