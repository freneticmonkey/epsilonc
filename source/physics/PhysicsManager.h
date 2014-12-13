#pragma once
//
//  ResourceManager.h
//  Epsilon
//
//  Created by Scott Porter on 26/04/2014.
//
#include <map>
#include <vector>
#include <algorithm>

#include "EpsilonCore.h"

#include "physics/PhysicsEvents.h"
#include "physics/RigidBody.h"
#include "physics/Collider.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace epsilon
{
	/**
	* PhysicsManager
	*
	* This class wraps interaction by the rest of the engine with the Physics simulation.  
	* The physics simulation is performed by bullet physics.
	* http://bulletphysics.org/
	*/
	class PhysicsManager
	{
		// The RigidBody components
		typedef std::map<long, RigidBody::Ptr>	RigidBodies;
		// The Bullet RigidBody pointers created by the components
		typedef std::map<long, btRigidBody *>	RigidBodyPtrs;

	private:
		PhysicsManager(void);

	public:
		static PhysicsManager & GetInstance()
		{
			static PhysicsManager instance;
			return instance;
		}
		~PhysicsManager(void);

		void SetUpdateFrequency(float updateFreq)
		{
			updateFrequency = updateFreq;
		}

		// Create the simulation and ready it for rigidbodies.
		void Setup();

		// Updates the Physics simulation
		void Update(float dt);
        
        // Push bullet updates back to epsilon Transforms
        void UpdateFromBullet();
        
        // Push updates from scripts to bullet
        void UpdateToBullet();

		// Stop and delete the physics sim
		void Destroy();

		// Check world for collisions
		void ProcessCollisions();

		// Bullet Collision Callbacks
		bool ContactAdded(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
		bool ContactProcessed(btManifoldPoint& cp, void * body0, void * body1);
		bool ContactDestroyed(void * userPersistentData);

		// RigidBody Creation
		RigidBody::Ptr CreateRigidBody(float mass = 0.f, Vector3 inertia = Vector3::ZERO, bool kinematic = false);
		RigidBody::Ptr GetRigidBody(long id);

		// Track RigidBody state changes
		void RigidBodyCreated(long rigidbodyId, btRigidBody	* rigidBody);
		void RigidBodyDeleted(long rigidbodyId);

		// Read Only for now.
		const Vector3& GetGravity() { return gravity;  }
        
        // Physics functions
        // Raycast
		// SphereCast

	private:
		RigidBodies			 rigidBodies;
		RigidBodyPtrs		 rigidBodyPtrs;

		// Callback function objects to use with registering the RigidBodies
		RigidBody::RigidBodyCreatedCallback onCreateRigidBody;
		RigidBody::RigidBodyDeletedCallback onDeleteRigidBody;

		// The Frequency in seconds between physics simualation updates (diabled by default)
		float				updateFrequency;
		float				elSinceCheck;

		Vector3				gravity;

		// Bullet Physics
		btDefaultCollisionConfiguration		* collisionConfiguration;
		btCollisionDispatcher				* dispatcher;
		btBroadphaseInterface				* overlappingPairCache;
		btSequentialImpulseConstraintSolver	* solver;
		btDiscreteDynamicsWorld				* dynamicsWorld;

		btCollisionWorld					* collisionWorld;
	};

	bool OnCollisionAdded(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
	bool OnCollisionProcessed(btManifoldPoint& cp, void * body0, void * body1);
	bool OnCollisionDestroyed(void * userPersistentData);
}
