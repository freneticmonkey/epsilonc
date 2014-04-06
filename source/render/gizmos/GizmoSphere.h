#pragma once

#include "render/gizmos/GizmoType.h"
#include "render/gizmos/GizmoOperation.h"

namespace epsilon
{
	class GizmoSphere : virtual public GizmoType
	{
	private:
		GizmoSphere(void);

	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;

		static GizmoSphere & GetInstance()
		{
			static GizmoSphere instance;
			return instance;
		}
		~GizmoSphere(void);

		Matrix4 ExtractTransform(GizmoOperation::Ptr op);

	};
}