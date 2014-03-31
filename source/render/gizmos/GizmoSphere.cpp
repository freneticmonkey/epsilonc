#include "render/gizmos/GizmoSphere.h"
#include "render/MeshFactory.h"
#include "render/material/MaterialManager.h"

namespace epsilon
{
	GizmoSphere::GizmoSphere()
	{
		mesh = MeshFactory::GenerateWireSphere();
		material = MaterialManager::GetInstance().GetMaterialByName("default");
	}

	GizmoSphere::~GizmoSphere()
	{

	}

	void GizmoSphere::Draw(RenderStateStack::Ptr stateStack)
	{
		std::for_each(operations[currentOperations].begin(), operations[currentOperations].end(), [&](GizmoOperation::Ptr op){
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