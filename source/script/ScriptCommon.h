#pragma once
#include "EpsilonCore.h"

// Exposing std::shared_ptr to boost

/*
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
*/
#include <boost/python.hpp>

# include <boost/python/handle.hpp>
# include <boost/python/converter/shared_ptr_deleter.hpp>
# include <boost/python/converter/rvalue_from_python_data.hpp>
# include <boost/python/converter/registered.hpp>

//# include <boost/python/converter/from_python.hpp>

using namespace boost::python; 

using namespace epsilon;
 
// Testing std::shared_ptr <-> type conversion handlers
template<typename T>
struct std_shared_ptr_from_python_converter
{
	std_shared_ptr_from_python_converter()
	{
		converter::registry::push_back(&convertible, &construct, type_id<std::shared_ptr<T> >() );
	}

	static void * convertible(PyObject *obj)
	{
		if (obj == Py_None)
            return obj;
        
        return converter::get_lvalue_from_python(obj, converter::registered<T>::converters);
	}

	static void construct(PyObject *source, boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		 void* const storage = ((converter::rvalue_from_python_storage<std::shared_ptr<T> >*)data)->storage.bytes;
        
		 // Deal with the "None" case.
        if (data->convertible == source)
            new (storage) std::shared_ptr<T>();
        else
        {
            std::shared_ptr<void> hold_convertible_ref_count(
              (void*)0, 
			  boost::python::converter::shared_ptr_deleter(handle<>(borrowed(source))) );
            // use aliasing constructor
            new (storage) std::shared_ptr<T>(
                hold_convertible_ref_count,
                static_cast<T*>(data->convertible));
        }
        
        data->convertible = storage;
	}
};

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

		formatted = python::str("\n").join(formatted_list);
		Log( extract<std::string>(formatted) );
	}
}