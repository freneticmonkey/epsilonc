#pragma once
#include "EpsilonCore.h"
#include "render/material/Material.h"
#include "render/mesh/Mesh.h"
#include "render/gizmos/GizmoOperation.h"
#include <atomic>

namespace epsilon
{
	// Base class for managing a gizmo type
	class GizmoType
	{
	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;
		
		explicit GizmoType(void);
		~GizmoType(void);

		void Update(float el);
		
		virtual Matrix4 ExtractTransform(GizmoOperation::Ptr op) { return Matrix4();  };

		void Draw();
        
        void Destroy();

		void QueueOperation(const GizmoOperation::Ptr op);

	protected:

		GizmoOperations		operations[2];
		std::atomic<int>	currentOperations;

		Material::Ptr			material;
		Mesh::Ptr				mesh;
	};
}

