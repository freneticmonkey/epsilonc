#pragma once

#include "EpsilonCore.h"

#include "render/gizmos/GizmoType.h"
#include "render/gizmos/GizmoCube.h"
#include "render/gizmos/GizmoSphere.h"
#include "render/gizmos/GizmoLine.h"
#include "render/gizmos/GizmoAxis.h"

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
		void Draw();
		void Update(float el);
        void Destroy();
		GizmoManager & AddGizmo(GizmoType * newGizmo);

		// Draw Gizmos
		static void SetColour(Colour newColour);
		static void SetLife(float newLife);
		static void DrawCube(Vector3 position, Vector3 size);
		static void DrawSphere(Vector3 position, float radius);
		static void DrawLine(Vector3 from, Vector3 to);
		static void DrawAxisMatrix(Matrix4 mat);
		static void DrawAxisVectors(Vector3 pos, Vector3 right, Vector3 up, Vector3 forward);

	private:
		Colour			gizmoColour;
		float			gizmoLife;

		GizmoTypes		gizmos;
		GizmoCube	*	gizmoCube;
		GizmoSphere *	gizmoSphere;
		GizmoLine	*	gizmoLine;
		GizmoAxis	*	gizmoAxis;
	};
}



