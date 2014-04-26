//
//  ResourceManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//
#include "physics/PhysicsManager.h"

namespace epsilon
{
	PhysicsManager::PhysicsManager(void) : elSinceCheck(0),
											updateFrequency(0.f),
											onCreateRigidBody(),
											gravity(0, -9.8f, 0)
	{
		onCreateRigidBody = std::bind(&PhysicsManager::RigidBodyCreated, this, std::placeholders::_1, std::placeholders::_2);
		onDeleteRigidBody = std::bind(&PhysicsManager::RigidBodyDeleted, this, std::placeholders::_1);
	}

	PhysicsManager::~PhysicsManager(void)
	{
		rigidBodies.clear();
	}

	void PhysicsManager::Setup()
	{
		// Grabbed straight from the bullet hello world demo :)

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new	btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		
		// Set gravity
		dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	void PhysicsManager::Destroy()
	{
		//delete dynamics world
		delete dynamicsWorld;

		//delete solver
		delete solver;

		//delete broadphase
		delete overlappingPairCache;

		//delete dispatcher
		delete dispatcher;

		delete collisionConfiguration;
	}

	void PhysicsManager::Update(float dt)
	{
		// If the update frequency is enabled.
		if (updateFrequency > 0.f)
		{
			// Check if it is time to update the physics simualation
			elSinceCheck += dt;
			if (elSinceCheck >= updateFrequency)
			{
				// Update simualation
				dynamicsWorld->stepSimulation(dt, 10);

				// Update the SceneNode's with RigidBodies attached.
				std::for_each(rigidBodies.begin(), rigidBodies.end(), [](std::pair<long, RigidBody::Ptr> rigidbody){
					rigidbody.second->Update();
				});

				// Reset the wait period :)
				elSinceCheck = 0.f;
			}
		}
		else
		{
			// Update simualation
			dynamicsWorld->stepSimulation(dt, 10);

			// Update the SceneNode's with RigidBodies attached.
			std::for_each(rigidBodies.begin(), rigidBodies.end(), [](std::pair<long, RigidBody::Ptr> rigidbody){
				rigidbody.second->Update();
			});
		}
	}

	RigidBody::Ptr PhysicsManager::CreateRigidBody(float mass, Vector3 inertia)
	{
		RigidBody::Ptr newRB = RigidBody::Create(mass, inertia);
		rigidBodies[newRB->GetId()] = newRB;

		// Register state change callbacks
		newRB->OnCreateRegister(onCreateRigidBody);
		newRB->OnDeleteRegister(onDeleteRigidBody);
		
		// Now create the bullet rigidbody pieces
		newRB->Setup();

		return newRB;
	}

	RigidBody::Ptr PhysicsManager::GetRigidBody(long id)
	{
		RigidBody::Ptr result;

		RigidBodies::iterator body = rigidBodies.find(id);

		if (body != rigidBodies.end())
		{
			result = rigidBodies[id];
		}

		return result;
	}

	void PhysicsManager::RigidBodyCreated(long rigidbodyId, btRigidBody	* rigidBody)
	{
		// Remove any existing rigidbody first
		RigidBodyDeleted(rigidbodyId);

		// Add to physics simulation
		dynamicsWorld->addRigidBody(rigidBody);

		// Add to tracking map
		rigidBodyPtrs[rigidbodyId] = rigidBody;
	}

	void PhysicsManager::RigidBodyDeleted(long rigidbodyId)
	{
		
		RigidBodyPtrs::iterator found = rigidBodyPtrs.find(rigidbodyId);

		// If we already have a rigidbody for this rigid body, 
		// remove it from the simulation first
		if (found != rigidBodyPtrs.end())
		{
			// Remove from physics simuation
			dynamicsWorld->removeRigidBody(rigidBodyPtrs[rigidbodyId]);

			// Remove from the map;
			rigidBodyPtrs.erase(found);
		}
	}
}

