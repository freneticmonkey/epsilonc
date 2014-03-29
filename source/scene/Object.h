#pragma once

#include "EpsilonCore.h"

#include <string>
#include <sstream>

namespace epsilon
{
	class Object;

	class Object //: public enable_shared_from_this<Object>
	{
	public:
		//typedef std::shared_ptr<Object> Ptr;

		long GetId() { return id; }
		const std::string GetName() { return objectName; }
		std::string GetClass() { return className; }
        bool IsEnabled() { return enabled; }
        void Enable()
        {
            enabled = true;
            OnEnable();
        }
        void Disable()
        {
            enabled = false;
            OnDisable();
        }
        
        // These functions can be overridden by inheriting classes
        // in order to responded to enabled/disabled state
        void OnEnable() {};
        void OnDisable() {};

		Object() : enabled(true)
		{
			std::stringstream ss;

			id = Object::GenId();
			ss << id;
			className = "Object";
			objectName = className + std::string("_") + ss.str();
		}

		Object( std::string newClassName ) : enabled(true)
		{
			std::stringstream ss;

			id = Object::GenId();
			ss << id;
			className = newClassName;
			objectName = className + std::string("_") + ss.str();
		}

		Object(std::string name, std::string newClassName ) : enabled(true)
		{
			id = Object::GenId();
			className = newClassName;
			objectName = name;
		}

		void SetName(std::string name)
		{
			objectName = name;
		}

		//Object::Ptr GetObjectPtr() { return ThisPtr(); }

	protected:
		long id;
		std::string objectName;
		std::string className;
        bool enabled;

	private:
		static long GenId() 
		{ 
			static long _id = 0;
			return _id++; 
		}

		//Object::Ptr ThisPtr() { return shared_from_this(); }
		
	};
}