#include "render/gizmos/GizmoCube.h"
#include "render/mesh/MeshFactory.h"

namespace epsilon
{
	GizmoCube::GizmoCube()
	{
		mesh = MeshFactory::GenerateWireCube();
	}

	GizmoCube::~GizmoCube()
	{

	}

	Matrix4 GizmoCube::ExtractTransform(GizmoOperation::Ptr op)
	{
		return Matrix4::CreateTranslation(op->vector[0].x,
											op->vector[0].y,
											op->vector[0].z)
									.Scale(op->vector[1].x,
											op->vector[1].y,
											op->vector[1].z);
	}
}