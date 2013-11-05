
#include <boost/python.hpp>

#include "script/python/Module.h"
#include "script/python/Math.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(epsilon)
{
	object package = scope();
	package.attr("__path__") = "epsilon";

	initMath();
}