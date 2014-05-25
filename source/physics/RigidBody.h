#pragma once
#include "EpsilonCore.h"

#include "math/Vector.h"
#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include <functional>

#include <btBulletDynamicsCommon.h>

namespace epsilon
{
	class RigidBody :
		public NodeComponent,
		public std::enable_shared_from_this<RigidBody>
	{
		friend class PhysicsManager;

	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<RigidBody>		Ptr;
		typedef std::function<void(long, btRigidBody *)>	RigidBodyCreatedCallback;
		typedef std::function<void(long)>					RigidBodyDeletedCallback;

		// RigidBodies can only be created by the PhysicsManager
		// to ensure that they are added to the simulation
		static RigidBody::Ptr Create(float mass = 0.f, Vector3 inertia = Vector3::ZERO, bool kinematic = false);

		explicit RigidBody(const private_struct &, float mass, Vector3 inertia, bool kinematic);
		~RigidBody(void);

		void OnSetParent();

		// Create Physics objects
		void Setup();

		void OnCreateRegister(const RigidBodyCreatedCallback& callback);
		void OnDeleteRegister(const RigidBodyDeletedCallback& callback);

		// Push Physics updates back to the Transform
		void UpdateFromBullet();
        
        // Push Transform updates back to bullet.
        void UpdateToBullet();

		// Accessors
		void	SetMass(float newMass);
		float	GetMass() { return mass; }

		void	SetKinematic(bool isKinematic);
		bool	IsKinematic() { return kinematic; }

		void	SetInertia(Vector3 newInertia);
		Vector3	GetInertia() { return inertia; }

		Vector3	GetLinearVelocity();
		void	SetLinearVelocity(Vector3 iLinearVelocity);

		Vector3	GetLinearDamping();
		void	SetLinearDamping(Vector3 iLinearDamping);

		Vector3	GetAngularVelocity();
		void	SetAngularVelocity(Vector3 iAngularVelocity);

		Vector3	GetAngularDamping();
		void	SetAngularDamping(Vector3 iAngularDamping);

		Vector3	GetTotalTorque();
		Vector3	GetTotalForce();

	private:
		RigidBody::Ptr ThisPtr() { return shared_from_this(); }

		bool	hasSetup;
		float	mass;
		Vector3 inertia;
		bool	kinematic;

		Vector3 linearVelocity;
		Vector3 linearDamping;
		Vector3 angularVelocity;
		Vector3 angularDamping;

		// The transform object
		Transform::Ptr			snTransform;

		// Bullet Objects
		btRigidBody				* rigidBody;
		btCollisionShape		* colShape;
		btDefaultMotionState	* myMotionState;
		btTransform				transform;

		// Callbacks to notify PhysicsManager of state changes
		RigidBodyCreatedCallback createdCallback;
		RigidBodyDeletedCallback deletedCallback;
	};

	// Declaring container
	typedef std::vector<RigidBody::Ptr> RigidBodies;
}