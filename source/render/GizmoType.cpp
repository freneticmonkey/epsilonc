#include <algorithm>

#include "render/gizmos/GizmoType.h"
#include "render/Mesh.h"

namespace epsilon
{
	GizmoType::GizmoType(void)
	{
	}

	GizmoType::~GizmoType(void)
	{
	}

	void GizmoType::QueueOperation(const GizmoOperation::Ptr op)
	{
		// Add it to the gizmos operations
		operations.push_back(op);
	}

	void GizmoType::Update(float el)
	{
		// Remove any expired gizmos
		for (GizmoOperations::iterator it = operations.begin(); it != operations.end();)
		{
			if ((*it)->HasExpired() && (*it)->HasRendered())
			{
				it = operations.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Update the life for each of the gizmos
		std::for_each(operations.begin(), operations.end(), [el](GizmoOperation::Ptr op){
			op->Update(el);
		});
	}
}