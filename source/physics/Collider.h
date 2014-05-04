#pragma once
#include "EpsilonCore.h"

#include "math/Vector.h"
#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include <functional>

#include <btBulletDynamicsCommon.h>

namespace epsilon
{
	class Collider :
		public NodeComponent
	{
		friend class PhysicsManager;

	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<Collider>		Ptr;

		typedef std::function<void(long, btCollisionShape *)>	ColliderCreatedCallback;
		typedef std::function<void(long)>						ColliderDeletedCallback;

		enum ColliderType
		{
			PLANE = 0,
			BOX,
			SPHERE,
			CAPSULE,
			MESH,
			TERRAIN
		};

		// RigidBodies can only be created by the PhysicsManager
		// to ensure that they are added to the simulation
		static Collider::Ptr CreatePlane(Vector3 normal = Vector3::ZERO, float constant = 1.f);
		static Collider::Ptr CreateBox(Vector3 dimensions = Vector3::ONE);
		static Collider::Ptr CreateSphere(float radius = 1.f);
		static Collider::Ptr CreateCapsule(float radius = 1.f, float height = 1.f);
		
		// NYI
		//static Collider::Ptr CreateMesh();
		//static Collider::Ptr CreateTerrain();

		explicit Collider(const private_struct &);
		explicit Collider(const private_struct &, Vector3 normal, float constant);
		explicit Collider(const private_struct &, Vector3 dimensions);
		explicit Collider(const private_struct &, float radius);
		explicit Collider(const private_struct &, float radius, float height);
		//explicit Collider(const private_struct &);
		//explicit Collider(const private_struct &);

		~Collider(void);

		void OnSetParent();

		// Create Physics objects
		void Setup();

		void OnCreateRegister(const ColliderCreatedCallback& callback);
		void OnDeleteRegister(const ColliderDeletedCallback& callback);

		// Push Physics updates back to the Transform
		void UpdateFromBullet();

		// Push Transform updates back to bullet.
		void UpdateToBullet();

	private:
		ColliderType	type;
		bool			hasSetup;

		// Intermediary Values
		Vector3			normal;
		Vector3			dimensions;
		float			constant;
		float			radius;
		float			height;

		// The transform object
		Transform::Ptr			snTransform;

		// Bullet Objects
		btCollisionShape		* colShape;
		
		// Callbacks to notify PhysicsManager of state changes
		ColliderCreatedCallback createdCallback;
		ColliderDeletedCallback deletedCallback;
	};

	// Declaring container
	typedef std::vector<Collider::Ptr> Colliders;
}