#include "render/gizmos/GizmoSphere.h"
#include "render/MeshFactory.h"
#include "render/material/MaterialManager.h"

namespace epsilon
{
	GizmoSphere::GizmoSphere()
	{
		mesh = MeshFactory::GenerateWireSphere();
		material = MaterialManager::GetInstance().GetMaterialByName("gizmo");
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