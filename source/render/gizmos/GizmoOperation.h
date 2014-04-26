#pragma once
#include "math/Vector.h"
#include "math/Matrix.h"
#include "render/Colour.h"

namespace epsilon
{
	// Base class for Gizmo Operations
	class GizmoOperation
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<GizmoOperation> Ptr;

		static GizmoOperation::Ptr Create()
		{
			return std::make_shared<GizmoOperation>(private_struct());
		}

		explicit GizmoOperation(const private_struct &) : life(0.0f),
														  hasRendered(false)
		{

		}
		~GizmoOperation(void) {}

		void	SetLife(float newLife)
		{
			life = newLife;
		}

		void	Update(float el) { life -= el; }
		void	MarkRendered() { hasRendered = true;  }

		bool	HasExpired() { return life <= 0.f; }
		bool	HasRendered() { return hasRendered; }

		// Operation parameters, usage determined by Gizmo Type
		Vector3 vector[4];
		float	value[3];
		
		// White by default
		Colour	colour;

	private:
		float	life;
		bool	hasRendered;
	};
}