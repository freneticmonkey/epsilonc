#pragma once
#include "EpsilonCore.h"
#include "render/RenderState.h"
#include "render/gizmos/GizmoOperation.h"
#include <atomic>

namespace epsilon
{
	// Base class for managing a gizmo type
	class GizmoType
	{
	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;
		
		explicit GizmoType(void);
		~GizmoType(void);

		void Update(float el);
		virtual void Draw(RenderStateStack::Ptr stateStack) {};

		void QueueOperation(const GizmoOperation::Ptr op);

	protected:

		GizmoOperations		operations[2];
		std::atomic<int>	currentOperations;
	};
}

