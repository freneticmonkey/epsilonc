#include "render/gizmos/GizmoSphere.h"
#include "render/MeshFactory.h"

namespace epsilon
{
	GizmoSphere::GizmoSphere()
	{
		mesh = MeshFactory::GenerateWireSphere();
	}

	GizmoSphere::~GizmoSphere()
	{

	}

	Matrix4 GizmoSphere::ExtractTransform(GizmoOperation::Ptr op)
	{
		return Matrix4::CreateTranslation(op->vector[0].x,
											op->vector[0].y,
											op->vector[0].z)
									.Scale(op->value[0],
											op->value[0],
											op->value[0]);
	}
}