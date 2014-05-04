#include "physics/Collider.h"

#include <boost/format.hpp>

#include <btBulletCollisionCommon.h>

using namespace boost;

namespace epsilon
{
	Collider::Ptr Collider::CreatePlane(Vector3 normal, float constant)
	{
		return std::make_shared<Collider>(private_struct(), normal, constant);
	}

	Collider::Ptr Collider::CreateBox(Vector3 dimensions)
	{
		return std::make_shared<Collider>(private_struct(), dimensions);
	}

	Collider::Ptr Collider::CreateSphere(float radius)
	{
		return std::make_shared<Collider>(private_struct(), radius);
	}

	Collider::Ptr Collider::CreateCapsule(float radius, float height)
	{
		return std::make_shared<Collider>(private_struct(), radius, height);
	}

	/*
	Collider::Ptr Collider::CreateMesh()
	{
		return std::make_shared<Collider>(private_struct());
	}

	Collider::Ptr Collider::CreateTerrain()
	{
		return std::make_shared<Collider>(private_struct());
	}
	*/

	Collider::Collider(const private_struct &, Vector3 normal, float constant) :
		NodeComponent("PlaneCollider"),
		hasSetup(false),
		type(ColliderType::PLANE),
		normal(normal),
		constant(constant)
	{
	}

	Collider::Collider(const private_struct &, Vector3 dimensions) :
		NodeComponent("BoxCollider"),
		hasSetup(false),
		type(ColliderType::BOX),
		dimensions(dimensions)
	{
	}

	Collider::Collider(const private_struct &, float radius) :
		NodeComponent("SphereCollider"),
		hasSetup(false),
		type(ColliderType::SPHERE),
		radius(radius)
	{
	}

	Collider::Collider(const private_struct &, float radius, float height) :
		NodeComponent("CapsuleCollider"),
		hasSetup(false),
		type(ColliderType::CAPSULE),
		radius(radius),
		height(height)
	{
	}

	/*
	Collider::Collider(const private_struct &) : 
		NodeComponent("MeshCollider"),
		hasSetup(false),
		type(ColliderType::MESH)
	{
	}

	Collider::Collider(const private_struct &) : 
		NodeComponent("TerrainCollider"),
		hasSetup(false),
		type(ColliderType::TERRAIN)
	{
	}
	*/

	Collider::~Collider(void)
	{
		deletedCallback(GetId());
	}

	void Collider::OnSetParent()
	{
		snTransform = this->componentParent->GetComponent<Transform>();
		Setup();
	}

	void Collider::OnCreateRegister(const ColliderCreatedCallback& callback)
	{
		createdCallback = callback;
	}

	void Collider::OnDeleteRegister(const ColliderDeletedCallback& callback)
	{
		deletedCallback = callback;
	}

	void Collider::Setup()
	{
		// If the rigid body hasn't yet been setup, and there is a parent transform
		if (!hasSetup && snTransform)
		{
			//create a dynamic Collider
			switch (type)
			{
				case ColliderType::PLANE:
					colShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), constant);
					break;
				case ColliderType::BOX:
					colShape = new btBoxShape(btVector3(dimensions.x, dimensions.y, dimensions.z));
					break;
				case ColliderType::SPHERE:
					colShape = new btSphereShape(radius);
					break;
				case ColliderType::CAPSULE:
					colShape = new btCapsuleShape(radius, height);
					break;
				case ColliderType::MESH:
					//colShape = new btTriangleMeshShape(/* Mesh */);	// NYI
					break;
				case ColliderType::TERRAIN:
					//colShape = new btHeightfieldTerrainShape(btVector3(0.5f, 0.5f, 0.5f)); // NYI
					break;
			}

			createdCallback(GetId(), colShape);

			hasSetup = true;
		}
	}

	void Collider::UpdateFromBullet()
	{
		if (hasSetup)
		{
			
		}
	}

	void Collider::UpdateToBullet()
	{
		if (hasSetup)
		{
			
		}
	}
}