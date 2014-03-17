#include "render/gizmos/GizmoType.h"
#pragma once
#include "render/Material.h"
#include "render/RenderState.h"
#include "render/Mesh.h"

#include "render/gizmos/GizmoOperation.h"

namespace epsilon
{	
	class GizmoCube : virtual public GizmoType
	{
	private:
		GizmoCube(void);

	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;

		static GizmoCube & GetInstance()
		{
			static GizmoCube instance;
			return instance;
		}
		~GizmoCube(void);

		void Draw(RenderStateStack::Ptr stateStack);

	private:
		Material::Ptr			material;
		Mesh::Ptr				mesh;

	};
}