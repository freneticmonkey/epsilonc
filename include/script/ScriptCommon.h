#pragma once
#include "EpsilonCore.h"

// Exposing std::shared_ptr to boost

namespace boost 
{
	template<class T> const T* get_pointer(const std::shared_ptr<T>& p)
	{
		return p.get();
	}

	template<class T> T* get_pointer(std::shared_ptr<T>& p)
	{
		return p.get();
	}
} // namespace boost

#include <boost/python.hpp>
using namespace boost::python;

namespace epsilon
{
	// http://stackoverflow.com/a/6576177
	// decode a Python exception into a string
	inline void PrintPythonError()
	{
		using namespace boost::python;
		using namespace boost;

		PyObject *exc,*val,*tb;
		object formatted_list, formatted;
		PyErr_Fetch(&exc,&val,&tb);
		handle<> hexc(exc),hval(allow_null(val)),htb(allow_null(tb)); 
		object traceback(import("traceback"));
		
		/*if (!tb) {
			object format_exception_only(traceback.attr("format_exception_only"));
			formatted_list = format_exception_only(hexc,hval);
		} else {*/
			object format_exception(traceback.attr("format_exception"));
			formatted_list = format_exception(hexc,hval,htb);
		//}

		formatted = str("\n").join(formatted_list);
		Log( extract<std::string>(formatted) );
	}


	//inline void PrintPythonError()
	//{
	//	// From: http://stackoverflow.com/a/1418703
	//		
	//	PyObject *ptype, *pvalue, *ptraceback;
	//	PyErr_Fetch(&ptype, &pvalue, &ptraceback);

	//	if ( ptype != NULL && pvalue != NULL && ptraceback != NULL )
	//	{
	//		handle<> hType(ptype);
	//		object extype(hType);
	//		handle<> hTraceback(ptraceback);
	//		object traceback(hTraceback);

	//		//Extract error message
	//		std::string strErrorMessage = extract<std::string>(pvalue);

	//		Log(Format("Python Load Error: %s", strErrorMessage));

	//		//Extract line number (top entry of call stack)
	//		// if you want to extract another levels of call stack
	//		// also process traceback.attr("tb_next") recurently
	//		long lineno = extract<long> (traceback.attr("tb_lineno"));
	//		std::string filename = extract<string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
	//		std::string funcname = extract<string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));

	//		// Using c_str as python strings don't have a trailing \0 (c_str adds one)
	//		Log(Format("%s: %s():%d", filename.c_str(), funcname.c_str(), lineno));

	//		Py_XDECREF(ptype);
	//		Py_XDECREF(pvalue);
	//		Py_XDECREF(ptraceback);
	//	}

	//	// Print to stdout
	//	PyErr_PrintEx(0);
	//}
}