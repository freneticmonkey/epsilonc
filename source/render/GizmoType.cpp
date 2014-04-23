#include <algorithm>

#include "render/gizmos/GizmoType.h"
#include "render/Mesh.h"
#include "render/material/MaterialManager.h"

namespace epsilon
{
	GizmoType::GizmoType(void) : currentOperations(0)
	{
		material = MaterialManager::GetInstance().GetMaterialByName("gizmo");
	}

	GizmoType::~GizmoType(void)
	{
	}

	void GizmoType::QueueOperation(const GizmoOperation::Ptr op)
	{
		// Add it to the gizmos operations
		operations[!currentOperations].push_back(op);
	}

	void GizmoType::Update(float el)
	{
		// Remove any expired gizmos
		for (GizmoOperations::iterator it = operations[currentOperations].begin(); it != operations[currentOperations].end();)
		{
			if ((*it)->HasExpired() && (*it)->HasRendered())
			{
				it = operations[currentOperations].erase(it);
			}
			else
			{
				++it;
			}
		}

		// Update the life for each of the gizmos
		std::for_each(operations[currentOperations].begin(), operations[currentOperations].end(), [el](GizmoOperation::Ptr op){
			op->Update(el);
		});

		currentOperations = !currentOperations;
	}
    
    void GizmoType::Destroy()
    {
        if ( mesh )
        {
            mesh->Destroy();
        }
    }

	void GizmoType::Draw()
	{
		std::for_each(operations[currentOperations].begin(), operations[currentOperations].end(), [&](GizmoOperation::Ptr op){
			// Configure material
			
			// Set Colour
			material->diffuse = op->colour;

			// Set Transformation Properties
			if (material->Enable(ExtractTransform(op)))
			{
				// Draw if the last draw was ok
				// OR if the last draw of this mesh was not ok, but the material has been refreshed, so we can try again.
				if (mesh->DrawOk() || (!mesh->DrawOk() && material->HasRefreshed()))
				{
					mesh->Draw();
				}
			}

			material->Disable();

			op->MarkRendered();
			//TODO: Draw Spheres using instancing

		});
		// Done.
	}
}