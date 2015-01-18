#include "physics/RigidBody.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	RigidBody::Ptr RigidBody::Create(float mass, Vector3 inertia, bool kinematic)
	{
		return std::make_shared<RigidBody>(private_struct(), mass, inertia, kinematic);
	}

	RigidBody::RigidBody(const private_struct &, float mass, Vector3 inertia, bool kinematic) : NodeComponent("RigidBody"),
																				mass(mass), 
																				inertia(inertia),
																				kinematic(kinematic),
																				hasSetup(false)
	{
	}

	RigidBody::~RigidBody(void)
	{
		OnDestroy();
	}

	void RigidBody::OnDestroy()
	{
		if (hasSetup)
		{
			deletedCallback(GetId());
			hasSetup = false;
		}
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

			// Configure the Collision options for the object

			int collisionFlags = rigidBody->getCollisionFlags();

			// Indicate that the rigidbody will fire a callback on collision
			collisionFlags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;

			// if kinematic
			if (kinematic)
			{
				collisionFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
			}

			rigidBody->setCollisionFlags( collisionFlags );

			// Attach a handle to this object to the rigidbody
			rigidBody->setUserPointer(ThisPtr().get());

			createdCallback(GetId(), rigidBody);

			hasSetup = true;
		}
	}

	void RigidBody::UpdateFromBullet()
	{
        if ( hasSetup )
        {
            // Get the position and rotation from bullet
            myMotionState->getWorldTransform(transform);
            
            btVector3 p = transform.getOrigin();
            btQuaternion r = transform.getRotation();
            
            // Update our transform with the latest physics info.
            snTransform->SetPosition(p.x(), p.y(), p.z());
            snTransform->SetOrientation(r.x(), r.y(), r.z(), r.w());
        }
	}
    
    void RigidBody::UpdateToBullet()
	{
        if ( hasSetup )
        {
            Vector3 pos = snTransform->GetPosition();
            Quaternion rot = snTransform->GetOrientation();
            btVector3 p(pos.x, pos.y, pos.z);
            btQuaternion r(rot.x, rot.y, rot.z, rot.w);
            transform.setOrigin(p);
            transform.setRotation(r);
            
            // Update the rigidbodies' position and rotation
            myMotionState->setWorldTransform(transform);
        }
	}

	void RigidBody::SetMass(float newMass)
	{
		mass = newMass;
		rigidBody->setMassProps(mass, btVector3(inertia.x, inertia.y, inertia.z));
	}

	void RigidBody::SetKinematic(bool isKinematic)
	{
		int collisionFlags = rigidBody->getCollisionFlags();

		// if kinematic
		if (isKinematic)
		{
			collisionFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
			kinematic = isKinematic;
		}
		else
		{
			if (kinematic)
			{
				collisionFlags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
			}
		}
	}

	void RigidBody::SetInertia(Vector3 newInertia)
	{
		inertia = newInertia;
		rigidBody->setMassProps(mass, btVector3(inertia.x, inertia.y, inertia.z));
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