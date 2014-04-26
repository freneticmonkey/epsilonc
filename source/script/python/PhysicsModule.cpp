#include "script/python/PhysicsModule.h"
#include "script/ScriptCommon.h"

#include "physics/PhysicsManager.h"
#include "physics/RigidBody.h"

void initPhysics()
{
	// make Physics package
	object physicsModule(handle<>(borrowed(PyImport_AddModule("epsilon.physics"))));
	scope().attr("physics") = physicsModule;

	scope physicsScope = physicsModule;
	
	class_<RigidBody, bases<NodeComponent>, RigidBody::Ptr, boost::noncopyable>("RigidBody", no_init)
		/*.add_property("mass", &RigidBody::GetMass, &RigidBody::SetMass)
		.add_property("inertia", &RigidBody::GetInertia, &RigidBody::SetInertia)

		.add_property("linear_velocity", &RigidBody::GetLinearVelocity, &RigidBody::SetLinearVelocity)
		.add_property("linear_damping", &RigidBody::GetLinearDamping, &RigidBody::SetLinearDamping)
		.add_property("angular_velocity", &RigidBody::GetAngularVelocity, &RigidBody::SetAngularVelocity)
		.add_property("angular_damping", &RigidBody::GetAngularDamping, &RigidBody::SetAngularDamping)

		.def_readonly("torque", &RigidBody::GetTotalTorque)
		.def_readonly("force", &RigidBody::GetTotalForce)
*/
		;

	implicitly_convertible<RigidBody::Ptr, NodeComponent::Ptr>();

	object physicsConstModule(handle<>(borrowed(PyImport_AddModule("epsilon.physics.const"))));
	physicsScope.attr("const") = physicsConstModule;
	scope constScope = physicsConstModule;

	// Add Render Constants
	constScope.attr("GRAVITY") = PhysicsManager::GetInstance().GetGravity();


}