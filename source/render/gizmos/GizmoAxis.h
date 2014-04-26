#pragma once
#include "render/gizmos/GizmoType.h"
#include "render/gizmos/GizmoOperation.h"

namespace epsilon
{
	class GizmoAxis : virtual public GizmoType
	{
	private:
		GizmoAxis(void);

	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;

		static GizmoAxis & GetInstance()
		{
			static GizmoAxis instance;
			return instance;
		}
		~GizmoAxis(void);

		// We're going to over-ride draw for Axis 
		// in order to use immediate mode drawing.
		void Draw();
	};
}