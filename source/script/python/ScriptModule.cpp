#include "script/python/RenderModule.h"

#include "script/ScriptCommon.h"
#include "scene/NodeComponent.h"
#include "script/Script.h"
#include "script/ScriptBehaviour.h"

void initScript()
{
	// make Render package
	object scriptModule( handle<>( borrowed( PyImport_AddModule("epsilon.script") ) ) );
	scope().attr("script") = scriptModule;

	scope scriptScope = scriptModule;

	enum_<ScriptSource>("ScriptSource")
		.value("NONE",  ScriptSource::NONE)
		.value("FILE", ScriptSource::FILE)
		.value("TEXT",  ScriptSource::TEXT)
	;

	class_<Script, bases<NodeComponent>, Script::Ptr, boost::noncopyable>("Script", no_init)
		// Not exposing any functions here so that Script Manager is the only place to create
		// Scripts.
		.def_readonly("filename", &Script::GetFilename)
		.def_readonly("text", &Script::GetText)
		.def_readonly("source_type", &Script::GetSource)
	;
	implicitly_convertible<Script::Ptr, NodeComponent::Ptr>();

	// Just declare this for now ... there isn't really a reason other than completeness
	class_<ScriptBehaviour, bases<Script, NodeComponent>, ScriptBehaviour::Ptr, boost::noncopyable>("ScriptBehaviour", no_init); 
	implicitly_convertible<ScriptBehaviour::Ptr, Script::Ptr>();
	//implicitly_convertible<ScriptBehaviour::Ptr, NodeComponent::Ptr>();



}