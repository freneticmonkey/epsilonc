#include "render/gizmos/GizmoType.h"
#pragma once
#include "render/Material.h"
#include "render/RenderState.h"
#include "render/Mesh.h"

#include "render/gizmos/GizmoOperation.h"

namespace epsilon
{
	class GizmoSphere : virtual public GizmoType
	{
	private:
		GizmoSphere(void);

	public:
		typedef std::vector<GizmoOperation::Ptr> GizmoOperations;

		static GizmoSphere & GetInstance()
		{
			static GizmoSphere instance;
			return instance;
		}
		~GizmoSphere(void);

		void Draw(RenderStateStack::Ptr stateStack);

	private:
		Material::Ptr			material;
		Mesh::Ptr				mesh;

	};
}