#pragma once
#include "render/gizmos/GizmoType.h"
#include "render/gizmos/GizmoOperation.h"

namespace epsilon
{
	class GizmoLine : virtual public GizmoType
	{
	private:
		GizmoLine(void);

	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;

		static GizmoLine & GetInstance()
		{
			static GizmoLine instance;
			return instance;
		}
		~GizmoLine(void);

		// We're going to over-ride draw for lines 
		// in order to use immediate mode drawing.
		void Draw();
	};
}