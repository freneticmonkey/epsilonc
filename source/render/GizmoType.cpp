#include <algorithm>

#include "render/gizmos/GizmoType.h"
#include "render/Mesh.h"

namespace epsilon
{
	GizmoType::GizmoType(void) : currentOperations(0)
	{
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
}