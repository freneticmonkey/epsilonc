#pragma once

#include "events/Event.h"
#include "events/EventListener.h"

#include "scene/SceneNode.h"
#include "physics/RigidBody.h"
#include "physics/Collider.h"

namespace epsilon
{
	class Collision : public EventDataBase
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<Collision> Ptr;
		static Collision::Ptr Create() { return std::make_shared<Collision>(private_struct()); }

		explicit Collision(const private_struct &) : EventDataBase("collision") {}

		Vector3			position;
		Vector3			normal;
		Collider::Ptr	collider;
		RigidBody::Ptr	rigidBody;

		static const std::size_t GetType() { return HashedString::Hash("collision"); }
	};

	typedef EventListener<Collision>			CollisionListener;
	typedef Event<Collision, CollisionListener> CollisionEvent;
}
