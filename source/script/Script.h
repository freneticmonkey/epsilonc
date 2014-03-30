#pragma once
#include "EpsilonCore.h"

#include "script/ScriptCommon.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include "render/Mesh.h"
#include "render/Shader.h"

#include "resource/Resource.h"

namespace epsilon
{
	enum ScriptSource
	{
		NONE = 1,
		FILE,
		TEXT,
	};

	class Script :
		public std::enable_shared_from_this<Script>,
		public NodeComponent,
		public Resource
	{
	protected:
		struct private_struct {};
		
	public:
		typedef std::shared_ptr<Script> Ptr;

		static Script::Ptr Create();
		static Script::Ptr CreateFromFile(std::string filename);
		static Script::Ptr CreateFromText(std::string scriptString);
		
		// Default constructor.  Possibly not needed
		explicit Script(const private_struct &);

		// Constructor for specifying a script from a file.
		explicit Script(const private_struct &, std::string filename);

		// Constructor for specifying a scirpt from text or 
		// from a file that isn't tracked by the resource hotloading system.
		explicit Script(const private_struct &, std::string scriptString, ScriptSource scriptSource);
		~Script(void);

		Script::Ptr ThisPtr() { return shared_from_this(); }

		// Listen to the on set parent event and inject the parent (Node)
		// into the scripts namespace for access by the script
		void OnSetParent();

		bool InitScript();

		// These functions might be the wrong way of going about things
		// Maybe scripts should only be created. If the source
		// changes, then create a new script? - Think about script names...
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
	protected:
		// Find a function in the Python Namespace parameter
		object FindPythonFunction(std::string funcName);
		
		// Override in derived classes in order to expose python functions
		virtual void RegisterScriptFunctions() {}
	private:	
		void RegisterPythonClass();

		bool initialised;
		dict scriptLocalNamespace;

		object behaviourClass;
		
		ScriptSource scriptSource;

		std::string filename;
		std::string text;

		// The name which any class instances must be called in Python
		// must be called to be recognised as the modules 'class'
		static const std::string INSTANCE_VAR_NAME;
	};

}