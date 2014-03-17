#pragma once

#include "EpsilonCore.h"

#include "render/RenderState.h"
#include "render/gizmos/GizmoType.h"
#include "render/gizmos/GizmoCube.h"
#include "render/gizmos/GizmoSphere.h"

namespace epsilon
{
	// Manages current gizmo operations.
	class GizmoManager
	{
		GizmoManager(void);

		typedef std::vector<GizmoType *> GizmoTypes;
	public:
		
		static GizmoManager & GetInstance()
		{
			static GizmoManager instance;
			return instance;
		}
		~GizmoManager(void);

		void Setup(void);
		void Draw(RenderStateStack::Ptr stateStack);
		void Update(float el);

		GizmoManager & AddGizmo(GizmoType * newGizmo);

		// Draw Gizmos
		static void SetColour(Colour newColour);
		static void SetLife(float newLife);
		static void DrawCube(Vector3 position, Vector3 size);
		static void DrawSphere(Vector3 position, float radius);

	private:
		
		RenderStateStack::Ptr stateStack;
		
		Colour			gizmoColour;
		float			gizmoLife;

		GizmoTypes		gizmos;
		GizmoCube  *	gizmoCube;
		GizmoSphere  *	gizmoSphere;
	};
}



