#include "render/gizmos/GizmoCube.h"
#include "render/MeshFactory.h"
#include "render/material/MaterialManager.h"

namespace epsilon
{
	GizmoCube::GizmoCube()
	{
		mesh = MeshFactory::GenerateWireCube();
		material = MaterialManager::GetInstance().GetMaterialByName("default");
	}

	GizmoCube::~GizmoCube()
	{

	}

	void GizmoCube::Draw(RenderStateStack::Ptr stateStack)
	{
		std::for_each(operations[currentOperations].begin(), operations[currentOperations].end(), [&](GizmoOperation::Ptr op){
				
			// Configure material
			stateStack->State()->model = Matrix4::CreateTranslation(op->vector[0].x,
																	op->vector[0].y,
																	op->vector[0].z)
															 .Scale(op->vector[1].x,
																	op->vector[1].y,
																	op->vector[1].z);
			// Set Colour
			material->diffuse = op->colour;
			
			// Set Transformation Properties
			material->Enable(stateStack);
				
			// Draw Cube
			mesh->Draw();

			material->Disable();

			op->MarkRendered();

			//TODO: Draw Cubes using instancing

		});
		

		// Done.
	}
}