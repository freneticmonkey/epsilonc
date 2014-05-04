//
//  ResourceManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//
#include "physics/PhysicsManager.h"
#include "utilities/EmptyDeleter.h"

#include "script/ScriptManager.h"

#include <btBulletCollisionCommon.h>

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

		// Create Collision detection members
		collisionWorld = new btCollisionWorld(dispatcher, overlappingPairCache, collisionConfiguration);

		// Register collision callbacks - Disabled because it's way too spammy
		//gContactAddedCallback = OnCollisionAdded;
		//gContactProcessedCallback = OnCollisionProcessed;
		//gContactDestroyedCallback = OnCollisionDestroyed;
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
				// Update bullet with the current state of the world
                UpdateToBullet();
                
                // Update simualation
                dynamicsWorld->stepSimulation(dt, 10);

				// Update collsions
				collisionWorld->performDiscreteCollisionDetection();
                
				// Update the world state post physics step
                UpdateFromBullet();

				// Look for Collisions
				ProcessCollisions();

				// Reset the wait period :)
				elSinceCheck = 0.f;
			}
		}
		else
		{
            // Update bullet with the current state of the world
            UpdateToBullet();
            
			// Update simualation
			dynamicsWorld->stepSimulation(dt, 10);

			// Update collsions
			collisionWorld->performDiscreteCollisionDetection();
            
			// Update the world state post physics step
			UpdateFromBullet();

			// Look for Collisions
			ProcessCollisions();
		}
	}
    
    void PhysicsManager::UpdateFromBullet()
    {
        // Update the SceneNode's with RigidBodies attached.
        std::for_each(rigidBodies.begin(), rigidBodies.end(), [](std::pair<long, RigidBody::Ptr> rigidbody){
            rigidbody.second->UpdateFromBullet();
        });
    }
    
    void PhysicsManager::UpdateToBullet()
    {
        // Update the SceneNode's with RigidBodies attached.
        std::for_each(rigidBodies.begin(), rigidBodies.end(), [](std::pair<long, RigidBody::Ptr> rigidbody){
            rigidbody.second->UpdateToBullet();
        });
    }

	void PhysicsManager::ProcessCollisions()
	{
		// This interdependency is not good.  Maybe queuing events which can be triggered by the ScriptManager?
		ScriptManager::GetInstance().LockGIL();

		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i<numManifolds; i++)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
			const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j<numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance()<0.f)
				{
					//NodeComponent::Ptr * componentA = static_cast<NodeComponent::Ptr*>(obA->getUserPointer());
					NodeComponent * componentA = static_cast<NodeComponent*>(obA->getUserPointer());
					NodeComponent * componentB = static_cast<NodeComponent*>(obB->getUserPointer());

					// Creating Shared Ptr's with custom deleters so that they can be passed into the 
					// collision event object
					NodeComponent::Ptr compA(componentA, empty_delete<NodeComponent>());
					NodeComponent::Ptr compB(componentB, empty_delete<NodeComponent>());

					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;

					Collision::Ptr newCollision = Collision::Create();
					newCollision->position = Vector3(ptB.x(), ptB.y(), ptB.z() );
					newCollision->normal = Vector3(normalOnB.x(), normalOnB.y(), normalOnB.z());

					// If the object hit was a RigidBody
					if (compB->GetClass() == "RigidBody")
					{
						newCollision->rigidBody = dynamic_pointer_cast<RigidBody>(compB);
					}
					// else if the object hit was a Collider
					else
					{
						newCollision->collider = dynamic_pointer_cast<Collider>(compB);
					}

					// Immediately send the event to be processed
					componentA->AddParentEvent(newCollision);

					// Reset the collided objects and sent to the other collider

					// If the object hit was a RigidBody
					if (compA->GetClass() == "RigidBody")
					{
						newCollision->rigidBody = dynamic_pointer_cast<RigidBody>(compA);
						newCollision->collider = nullptr;
					}
					// else if the object hit was a Collider
					else
					{
						newCollision->collider = dynamic_pointer_cast<Collider>(compA);
						newCollision->rigidBody = nullptr;
					}

					componentB->AddParentEvent(newCollision);
				}
			}
		}

		// Release the GIL
		ScriptManager::GetInstance().ReleaseGIL();
	}

	bool PhysicsManager::ContactAdded(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		return false;
	}

	bool PhysicsManager::ContactProcessed(btManifoldPoint& cp, void * body0, void * body1)
	{
		return false;
	}

	bool PhysicsManager::ContactDestroyed(void * userPersistentData)
	{
		return false;
	}

	RigidBody::Ptr PhysicsManager::CreateRigidBody(float mass, Vector3 inertia, bool kinematic)
	{
		RigidBody::Ptr newRB = RigidBody::Create(mass, inertia, kinematic);
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

		// Add to dynamics simulation
		dynamicsWorld->addRigidBody(rigidBody);

		// Add to collision world
		//collisionWorld->addCollisionObject(rigidBody);

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


	// Collision Callback Functions
	bool OnCollisionAdded(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		return PhysicsManager::GetInstance().ContactAdded(cp, colObj0Wrap, partId0, index0, colObj1Wrap, partId1, index1);
	}

	bool OnCollisionProcessed(btManifoldPoint& cp, void * body0, void * body1)
	{
		return PhysicsManager::GetInstance().ContactProcessed(cp, body0, body1);
	}

	bool OnCollisionDestroyed(void * userPersistentData)
	{
		return PhysicsManager::GetInstance().ContactDestroyed(userPersistentData);
	}
}

