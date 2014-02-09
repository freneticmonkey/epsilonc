#include "script/python/UtilitiesModule.h"
#include "script/PythonLogListener.h"


#ifdef _MSC_VER 
#include <intrin.h>
#include <Windows.h>
#endif

// Expose a method for triggering a break point 
void TriggerDebugBreak()
{
#ifdef _DEBUG

#ifdef _MSC_VER
	// if visual studio and a debugger is attached, otherwise ignore
	if (IsDebuggerPresent())
	{
		__debugbreak();
	}
#endif

#endif
}

// Expose current compile mode to python
bool IsDebug()
{
	bool debug = false;
#ifdef _DEBUG
	debug = true;
#endif
	return debug;
}

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

	def("debug_break", TriggerDebugBreak);
	def("is_debug", IsDebug);
}