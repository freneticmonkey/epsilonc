#pragma once
#include "EpsilonCore.h"

#include <boost/python.hpp>
using namespace boost::python;

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include "render/Mesh.h"
#include "render/Shader.h"

namespace epsilon
{
	enum ScriptSource
	{
		NONE = 1,
		FILE,
		TEXT,

	};

	class Script :
		public enable_shared_from_this<Script>,
		public NodeComponent
	{
	protected:
		struct private_struct {};

	public:
		typedef shared_ptr<Script> Ptr;

		static Script::Ptr Create();
		static Script::Ptr Create(std::string scriptString, ScriptSource source = ScriptSource::TEXT);
		
		explicit Script(const private_struct &);
		explicit Script(const private_struct &, std::string scriptString, ScriptSource scriptSource);
		~Script(void);

		Script::Ptr ThisPtr() { return shared_from_this(); }

		void InitScript(dict pythonGlobalNamespace);

		Script::Ptr SetScriptFile(std::string scriptFilename);
		Script::Ptr SetScriptText(std::string scriptText);

		std::string GetFilename() { return filename; }
		std::string GetText() { return text; }

		ScriptSource GetSource() { return scriptSource; }

		// Simple Function Calling
		/* 
		// Not implemented for now this can be created in future 
		// if dynamic function calls need to be made.  Probably should just 
		// implement a generic events system both on the C++ & python sides.

		Script::Ptr RegisterFunction(std::string funcName);
		bool Call(std::string funcName);
		bool Call(std::string funcName, FuncParams params);
		*/

		// Script Functions
		// These should be moved to a derived class.. but for now

		//void OnStart();
		//void Update(float dt);
		//void OnDestroy();
		//void OnDrawGizmos();

	protected:
		// Find a function in the Python Namespace parameter
		object FindPythonFunction(std::string funcName, dict pythonNamespace);
		
		// Override in derived classes in order to expose python functions
		virtual void RegisterScriptFunctions(dict pythonNamespace);

	private:	
		bool initialised;

		//object startFunction;
		//object updateFunction;
		//object destroyFunction;
		//object drawGizmosFunction;
		
		ScriptSource scriptSource;

		std::string filename;
		std::string text;
	};

}