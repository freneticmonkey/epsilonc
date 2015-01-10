#include "script/python/SceneModule.h"

#include "scene/Node.h"
#include "scene/NodeComponent.h"
#include "scene/Object.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/Transform.h"
#include "script/Script.h"
#include "physics/RigidBody.h"

#include "audio/AudioSource.h"
#include "audio/AudioListener.h"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	TranslateVector, Transform::Translate, 1, 2
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	TranslateFloat, Transform::Translate, 3, 4
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	TranslateMatrixVector, Transform::Translate, 2, 3
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	TranslateMatrixFloat, Transform::Translate, 4, 5
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	Roll, Transform::Roll, 1, 2
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	Pitch, Transform::Pitch, 1, 2
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	Yaw, Transform::Yaw, 1, 2
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	RotateAxis, Transform::Rotate, 2, 3
)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	RotateQuat, Transform::Rotate, 1, 2
)

void initScene()
{
	// make Scene package
	object sceneModule( handle<>( borrowed( PyImport_AddModule("epsilon.scene") ) ) );
	scope().attr("scene") = sceneModule;

	scope sceneScope = sceneModule;

	Node::Ptr (*NodeCreateStandard)() = &Node::Create;
	Node::Ptr (*NodeCreateNamed)(std::string) = &Node::Create;

	//register_ptr_to_python<Node::Ptr>();

	// This Class will never be constructed from Python, its methods are exposed
	// for object properties
	class_<Object>("Object", no_init)
		.def("id", &Object::GetId)
		.def("classname", &Object::GetClass)
		.add_property("name", &Object::GetName, &Object::SetName)
	;

	class_<NodeComponent, bases<Object>, NodeComponent::Ptr, boost::noncopyable>("NodeComponent", no_init)
		.add_property("parent", 
			&NodeComponent::GetParent,
			&NodeComponent::SetParent)
	;

	class_<Node, bases<NodeComponent>, Node::Ptr, boost::noncopyable>("Node", no_init)
		.def("create",NodeCreateStandard)
		.def("create",NodeCreateNamed)
		.staticmethod("create")

		.def("add_component", &Node::AddComponent)
		.def("remove_component", &Node::RemoveComponent)

		.def(self == other<Node::Ptr>())
		.def(self == long())
		.def(self == std::string())
	;

	class_<SceneNode, bases<Node>, SceneNode::Ptr, boost::noncopyable>("SceneNode", no_init)
		.def("add_child", (void(SceneNode::*)(SceneNode::Ptr)) &SceneNode::ScriptAddChild)

		.def("create_child", (SceneNode::Ptr(SceneNode::*)()) &SceneNode::ScriptCreateChild)
		.def("create_child", (SceneNode::Ptr(SceneNode::*)(std::string)) &SceneNode::ScriptCreateChild)
    
        .def("remove_child", (void(SceneNode::*)(std::string)) &SceneNode::RemoveChild)
        .def("remove_child", (void(SceneNode::*)(SceneNode::Ptr)) &SceneNode::RemoveChild)
    
        .def("remove_all_children", &SceneNode::RemoveAllChildren)
    
		.def("create_light", (Light::Ptr(SceneNode::*)()) &SceneNode::ScriptCreateLight)
		.def("create_light", (Light::Ptr(SceneNode::*)(std::string)) &SceneNode::ScriptCreateLight)
        
		.def("create_camera", (Camera::Ptr(SceneNode::*)()) &SceneNode::ScriptCreateCamera)
		.def("create_camera", (Camera::Ptr(SceneNode::*)(std::string)) &SceneNode::ScriptCreateCamera)
        
		.def("create_renderer", (Renderer::Ptr(SceneNode::*)()) &SceneNode::ScriptCreateRenderer)

		.def("create_behaviour", (ScriptBehaviour::Ptr(SceneNode::*)(std::string)) &SceneNode::ScriptCreateBehaviour)
		.def("create_rigidbody", (RigidBody::Ptr(SceneNode::*)(float, Vector3, bool))&SceneNode::ScriptCreateRigidBody, 
								 (python::arg("mass") = (float)(0.f)), 
								 (python::arg("inertia") = (Vector3)(Vector3::ZERO)),
								 (python::arg("kinematic") = (bool)(false)) )
    
        .def("create_audiosource", (AudioSource::Ptr(SceneNode::*)(std::string)) &SceneNode::CreateAudioSource, (python::arg("path") = (std::string)("")))
        .def("set_audiolistener", (AudioListener::Ptr(SceneNode::*)())&SceneNode::ScriptSetAudioListener)
		.def("is_audiolistener", &SceneNode::ScriptIsAudioListener)

		.add_property("transform", &SceneNode::GetTransform)
		.add_property("camera", &SceneNode::GetCamera)
		.add_property("light", &SceneNode::GetLight)
		.add_property("renderer", &SceneNode::GetRenderer)
		.add_property("scripts", &SceneNode::GetScripts)
		.add_property("rigidbody", &SceneNode::GetRigidBody)

		.def("get_script_by_name", &SceneNode::GetScriptByClassname)
		.def("get_scripts_by_name", &SceneNode::GetScriptsByClassname)

        .add_property("audiosources", &SceneNode::GetAudioSources)
    
        .def("get_audiosource_by_name", &SceneNode::GetAudioSourceByName)
    
        .def(self == other<SceneNode::Ptr>())
        .def(self == long())
        .def(self == std::string())
	;
    
	enum_<Transform::TransformSpace>("TransformSpace")
    .value("TS_LOCAL",  Transform::TransformSpace::TS_LOCAL)
    .value("TS_PARENT", Transform::TransformSpace::TS_PARENT)
    .value("TS_WORLD",  Transform::TransformSpace::TS_WORLD)
	;

	void (Transform::*LookAtTarget)(Vector3) = &Transform::LookAt;
	void (Transform::*LookAtFromTo)(Vector3, Vector3) = &Transform::LookAt;
	void (Transform::*LookAtFromToComp)(float, float, float, float, float, float) = &Transform::LookAt;

	class_<Transform, bases<NodeComponent>, Transform::Ptr, boost::noncopyable>("Transform", no_init)

		.add_property("local_position", make_function(&Transform::GetLocalPosition, return_value_policy<reference_existing_object>()),
		(Transform::Ptr(Transform::*)(const Vector3&)) &Transform::SetLocalPosition)

		.add_property("local_orientation", make_function(&Transform::GetLocalOrientation, return_value_policy<reference_existing_object>()),
		(Transform::Ptr(Transform::*)(const Quaternion&))&Transform::SetLocalOrientation)

		.add_property("local_scale", make_function(&Transform::GetLocalScale, return_value_policy<reference_existing_object>()),
		(Transform::Ptr(Transform::*)(const Vector3&)) &Transform::SetLocalScale)

		.add_property("position",  make_function(&Transform::GetPosition, return_value_policy<reference_existing_object>()),
								  ( Transform::Ptr (Transform::*)( const Vector3&) ) &Transform::SetPosition )

		.add_property("orientation", make_function(&Transform::GetOrientation, return_value_policy<reference_existing_object>()),
								  (Transform::Ptr (Transform::*)( const Quaternion&) )&Transform::SetOrientation )

		.add_property("scale", make_function(&Transform::GetScale, return_value_policy<reference_existing_object>()),
							   ( Transform::Ptr (Transform::*)( const Vector3&) ) &Transform::SetScale )

		.add_property("forward", make_function(&Transform::Forward, return_value_policy<reference_existing_object>()) )
		.add_property("up", make_function(&Transform::Up, return_value_policy<reference_existing_object>()) )
		.add_property("right", make_function(&Transform::Right, return_value_policy<reference_existing_object>()) )

		.add_property("matrix", make_function(&Transform::_getFullTransform, return_value_policy<reference_existing_object>()) )

		.add_property("bounds", make_function(&Transform::GetOuterBounds, return_value_policy<reference_existing_object>()))
		
		.add_property("meshbounds", make_function(&Transform::GetMeshBounds, return_value_policy<reference_existing_object>()))

		.add_property("parent_transform", &Transform::GetParentTransform, &Transform::SetParentTransform)
    
        .add_property("children", &Transform::GetChildren)

		.def("add_child", &Transform::AddChild)
		.def("remove_child", &Transform::RemoveChild)
		.def("remove_all_children", &Transform::RemoveAllChildren)
		.def("find_child_with_name", &Transform::FindChildWithName)
		.def("find_child_with_name_recursive", &Transform::FindChildWithNameRecursive)
		.def("find_child_with_id", &Transform::FindChildWithId)
		.def("find_child_with_idrecursive", &Transform::FindChildWithIdRecursive)
		.def("get_children", &Transform::GetChildren)
		.def("has_children", &Transform::HasChildren)

		.def("get_orientation", &Transform::GetOrientation, return_value_policy<reference_existing_object>())
		.def("reset_orientation", &Transform::ResetOrientation)
		.def("set_inherit_orientation", &Transform::SetInheritOrientation)
		.def("get_inherit_orientation", &Transform::GetInheritOrientation)
		
		.def("get_position", &Transform::GetPosition, return_value_policy<reference_existing_object>())
		.def("get_scale", &Transform::GetScale, return_value_policy<reference_existing_object>())
		.def("set_inherit_scale", &Transform::SetInheritScale)
		.def("get_inherit_scale", &Transform::GetInheritScale)

		.def("roll", &Transform::Roll, Roll())
		.def("pitch", &Transform::Pitch, Pitch())
		.def("yaw", &Transform::Yaw, Yaw())
		
		.def("get_localAxes", &Transform::GetLocalAxes)
		.def("get_derived_orientation", &Transform::_getDerivedOrientation, return_value_policy<reference_existing_object>())
		.def("get_derived_position", &Transform::_getDerivedPosition, return_value_policy<reference_existing_object>())
		.def("get_derived_scale", &Transform::_getDerivedScale, return_value_policy<reference_existing_object>())
		.def("get_full_transform", &Transform::_getFullTransform, return_value_policy<reference_existing_object>())
		
		.def("update", &Transform::_update)
		.def("need_update", &Transform::needUpdate)
		.def("request_update", &Transform::requestUpdate)
		.def("cancel_update", &Transform::cancelUpdate)
		

		.def("set_orientation", (Transform::Ptr (Transform::*)( const Quaternion&) )&Transform::SetOrientation )
		.def("set_orientation", (Transform::Ptr (Transform::*)( float, float, float, float) )&Transform::SetOrientation )
		
		.def("set_position", ( Transform::Ptr (Transform::*)( const Vector3&) ) &Transform::SetPosition )
		.def("set_position", ( Transform::Ptr (Transform::*)( float, float, float)  ) &Transform::SetPosition )
		.def("set_scale", ( Transform::Ptr (Transform::*)( const Vector3&) ) &Transform::SetScale )
		.def("set_scale", ( Transform::Ptr (Transform::*)( float, float, float)  ) &Transform::SetScale )
		.def("scaled", ( Transform::Ptr (Transform::*)( const Vector3&) ) &Transform::Scaled )
		.def("scaled", ( Transform::Ptr (Transform::*)( float, float, float)  ) &Transform::Scaled )
		.def("translate", ( Transform::Ptr (Transform::*)( const Vector3&, Transform::TransformSpace)  ) &Transform::Translate, TranslateVector() )
		.def("translate", ( Transform::Ptr (Transform::*)( float, float, float, Transform::TransformSpace)  ) &Transform::Translate, TranslateFloat() )
		.def("translate", ( Transform::Ptr (Transform::*)( const Matrix3&, const Vector3&, Transform::TransformSpace)  ) &Transform::Translate, TranslateMatrixVector() )
		.def("translate", ( Transform::Ptr (Transform::*)( const Matrix3&, float, float, float, Transform::TransformSpace)  ) &Transform::Translate, TranslateMatrixFloat() )
		.def("rotate", ( Transform::Ptr (Transform::*)(const Vector3&, const float&, Transform::TransformSpace)  ) &Transform::Rotate, RotateAxis() )
		.def("rotate", ( Transform::Ptr (Transform::*)(const Quaternion&, Transform::TransformSpace)  ) &Transform::Rotate, RotateQuat() )
		
		.def("lookat", LookAtTarget)
		.def("lookat", LookAtFromTo)
		.def("lookat", LookAtFromToComp)

		.def("fps", &Transform::FPS)
	;
	implicitly_convertible<Transform::Ptr, NodeComponent::Ptr>();
    
    class_<TransformList>("Transforms")
        .def("__iter__", python::iterator<TransformList>())
        .def("__len__", &TransformList::size)
    ;

	void (Scene::*SetActiveCameraPtr)(Camera::Ptr) = &Scene::SetActiveCamera;
	void (Scene::*SetActiveCameraName)(std::string) = &Scene::SetActiveCamera;

	class_<Scene, Scene::Ptr, boost::noncopyable>("Scene", no_init)
		.def("create", &Scene::Create)
		.staticmethod("create")
		
		.def("destroy", &Scene::Destroy)

		.def_readonly("name", &Scene::GetName)
		.def_readonly("root", &Scene::Root)
		.add_property("active_camera", &Scene::GetActiveCamera, SetActiveCameraPtr)
		.def("set_active_camera_name", SetActiveCameraName)

		.def("add_camera", &Scene::AddCamera)
		.def("get_camera", &Scene::GetCamera)


		.def(self == other<Scene::Ptr>())
		.def(self == std::string())
	;



}