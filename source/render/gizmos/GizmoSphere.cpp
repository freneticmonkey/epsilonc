#include "render/gizmos/GizmoSphere.h"
#include "render/MeshFactory.h"

namespace epsilon
{
	GizmoSphere::GizmoSphere()
	{
		mesh = MeshFactory::GenerateWireSphere();
		material = Material::Create();
	}

	GizmoSphere::~GizmoSphere()
	{

	}

	void GizmoSphere::Draw(RenderStateStack::Ptr stateStack)
	{
		std::for_each(operations.begin(), operations.end(), [&](GizmoOperation::Ptr op){
			// Configure material
			stateStack->State()->model = Matrix4::CreateTranslation(op->vector[0].x, 
																	op->vector[0].y, 
																	op->vector[0].z)
															 .Scale(op->value[0], 
																	op->value[0], 
																	op->value[0]);
			// Set Colour
			material->diffuse = op->colour;

			// Set Transformation Properties
			material->Enable(stateStack);

			// Draw Sphere
			mesh->Draw();

			material->Disable();

			op->MarkRendered();
			//TODO: Draw Spheres using instancing

		});
		// Done.
	}
}