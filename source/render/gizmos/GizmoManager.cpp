#include <algorithm>

#include "render/gizmos/GizmoManager.h"

namespace epsilon
{
	GizmoManager::GizmoManager() : gizmoLife(0.f)
	{
		
	}

	GizmoManager::~GizmoManager(void)
	{
	}

	void GizmoManager::Setup(void)
	{
		// Add Gizmos
		gizmoCube = &GizmoCube::GetInstance();
		AddGizmo(gizmoCube);
		gizmoSphere = &GizmoSphere::GetInstance();
		AddGizmo(gizmoSphere);
	}

	GizmoManager & GizmoManager::AddGizmo(GizmoType * newGizmo)
	{
		gizmos.push_back(newGizmo);
		return *this;
	}

	void GizmoManager::Draw()
	{
		// All Gizmos are drawn on top
		glDisable(GL_DEPTH_TEST);

		std::for_each(gizmos.begin(), gizmos.end(), [](GizmoType * gizmo){
			gizmo->Draw();
		});
	}
    
    void GizmoManager::Destroy()
    {
        std::for_each(gizmos.begin(), gizmos.end(), [](GizmoType * gizmo){
			gizmo->Destroy();
		});
    }

	void GizmoManager::Update(float el)
	{
		std::for_each(gizmos.begin(), gizmos.end(), [&](GizmoType * gizmo){
			gizmo->Update(el);
		});
	}

	void GizmoManager::SetColour(Colour newColour)
	{
		GizmoManager::GetInstance().gizmoColour = newColour;
	}

	void GizmoManager::SetLife(float newLife)
	{
		GizmoManager::GetInstance().gizmoLife = newLife;
	}

	void GizmoManager::DrawCube(Vector3 position, Vector3 size)
	{
		GizmoOperation::Ptr newCube = GizmoOperation::Create();

		newCube->colour = GizmoManager::GetInstance().gizmoColour;
		newCube->SetLife(GizmoManager::GetInstance().gizmoLife);
		newCube->vector[0] = position;
		newCube->vector[1] = size;

		GizmoManager::GetInstance().gizmoCube->QueueOperation(newCube);
	}

	void GizmoManager::DrawSphere(Vector3 position, float radius)
	{
		GizmoOperation::Ptr newSphere = GizmoOperation::Create();

		newSphere->colour = GizmoManager::GetInstance().gizmoColour;
		newSphere->SetLife(GizmoManager::GetInstance().gizmoLife);
		newSphere->vector[0] = position;
		newSphere->value[0] = radius;

		GizmoManager::GetInstance().gizmoSphere->QueueOperation(newSphere);
	}

}