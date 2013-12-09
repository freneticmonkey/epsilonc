#pragma once

#include "EpsilonCore.h"

#include <string>
#include <sstream>

namespace epsilon
{
	using namespace std;
	class Object;

	class Object //: public enable_shared_from_this<Object>
	{
	public:
		//typedef std::shared_ptr<Object> Ptr;

		long GetId() { return id; }
		const std::string GetName() { return objectName; }
		std::string GetClass() { return className; }

		Object()
		{
			stringstream ss;

			id = Object::GenId();
			ss << id;
			className = "Object";
			objectName = className + string("_") + ss.str();
		}

		Object( string newClassName )
		{
			stringstream ss;

			id = Object::GenId();
			ss << id;
			className = newClassName;
			objectName = className + string("_") + ss.str();
		}

		Object(string name, string newClassName ) 
		{
			id = Object::GenId();
			className = newClassName;
			objectName = name;
		}

		void SetName(string name)
		{
			objectName = name;
		}

		//Object::Ptr GetObjectPtr() { return ThisPtr(); }

	protected:
		long id;
		string objectName;
		string className;

	private:
		static long GenId() 
		{ 
			static long _id = 0;
			return _id++; 
		}

		//Object::Ptr ThisPtr() { return shared_from_this(); }
		
	};
}