
#include <boost/python.hpp>

#include "script/python/MainModule.h"
#include "script/python/LoggingModule.h"
#include "script/python/EventsModule.h"
#include "script/python/ManagerModule.h"
#include "script/python/MathModule.h"
#include "script/python/RenderModule.h"
#include "script/python/SceneModule.h"
#include "script/python/ScriptModule.h"
#include "script/python/UtilitiesModule.h"
#include "script/python/UIModule.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(epsilon)
{
	object package = scope();
	package.attr("__path__") = "epsilon";

	initLogging();
	initManagers();
	initEvents();
	initMath();
	initScene();
	initRender();
	initScript();
	initUtilities();
	initUI();
}