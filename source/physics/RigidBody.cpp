#include "physics/RigidBody.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	RigidBody::Ptr RigidBody::Create(float mass, Vector3 inertia)
	{
		return std::make_shared<RigidBody>(private_struct(), mass, inertia);
	}

	RigidBody::RigidBody(const private_struct &, float mass, Vector3 inertia) : NodeComponent("RigidBody"), 
																				mass(mass), 
																				inertia(inertia),
																				hasSetup(false)
	{
	}

	RigidBody::~RigidBody(void)
	{
		deletedCallback(GetId());
	}

	void RigidBody::OnSetParent()
	{
		snTransform = this->componentParent->GetComponent<Transform>();
		Setup();
	}

	void RigidBody::OnCreateRegister(const RigidBodyCreatedCallback& callback)
	{
		createdCallback = callback;
	}

	void RigidBody::OnDeleteRegister(const RigidBodyDeletedCallback& callback)
	{
		deletedCallback = callback;
	}

	void RigidBody::Setup()
	{
		// If the rigid body hasn't yet been setup, and there is a parent transform
		if (!hasSetup && snTransform)
		{
			//create a dynamic rigidbody

			//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
			colShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

			/// Create Dynamic Objects
			transform.setIdentity();

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass, localInertia);

			Vector3 position = snTransform->GetPosition();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
			Quaternion rot = snTransform->GetOrientation();
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			myMotionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

			rigidBody = new btRigidBody(rbInfo);

			createdCallback(GetId(), rigidBody);

			hasSetup = true;
		}
	}

	void RigidBody::Update()
	{
		// Get the position and rotation from bullet
		myMotionState->getWorldTransform(transform);

		btVector3 p = transform.getOrigin();
		btQuaternion r = transform.getRotation();

		// Update our transform with the latest physics info.
		snTransform->SetPosition(p.x(), p.y(), p.z());
		snTransform->SetOrientation(r.x(), r.y(), r.z(), r.w());
	}

	Vector3	RigidBody::GetLinearVelocity()
	{
		return Vector3::ZERO;
	}

	void RigidBody::SetLinearVelocity(Vector3 iLinearVelocity)
	{

	}

	Vector3	RigidBody::GetLinearDamping()
	{
		return Vector3::ZERO;
	}

	void RigidBody::SetLinearDamping(Vector3 iLinearDamping)
	{

	}

	Vector3	RigidBody::GetAngularVelocity()
	{
		return Vector3::ZERO;
	}

	void RigidBody::SetAngularVelocity(Vector3 iAngularVelocity)
	{

	}

	Vector3	RigidBody::GetAngularDamping()
	{
		return Vector3::ZERO;
	}

	void RigidBody::SetAngularDamping(Vector3 iAngularDamping)
	{

	}

	Vector3	RigidBody::GetTotalTorque()
	{
		return Vector3::ZERO;
	}

	Vector3	RigidBody::GetTotalForce()
	{
		return Vector3::ZERO;
	}
}