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
		gizmoLine = &GizmoLine::GetInstance();
		AddGizmo(gizmoLine);
		gizmoAxis = &GizmoAxis::GetInstance();
		AddGizmo(gizmoAxis);
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

	void GizmoManager::OnUpdate(float el)
	{
		std::for_each(gizmos.begin(), gizmos.end(), [&](GizmoType * gizmo){
			gizmo->Update(el);
		});
	}

	void GizmoManager::SetColour(Colour newColour)
	{
		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoColour = newColour;
	}

	void GizmoManager::SetLife(float newLife)
	{
		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoLife = newLife;
	}

	void GizmoManager::DrawCube(Vector3 position, Vector3 size)
	{
		GizmoOperation::Ptr newCube = GizmoOperation::Create();

		newCube->colour = GizmoManager::GetInstance().gizmoColour;
		newCube->SetLife(GizmoManager::GetInstance().gizmoLife);
		newCube->vector[0] = position;
		newCube->vector[1] = size;

		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoCube->QueueOperation(newCube);
	}

	void GizmoManager::DrawSphere(Vector3 position, float radius)
	{
		GizmoOperation::Ptr newSphere = GizmoOperation::Create();

		newSphere->colour = GizmoManager::GetInstance().gizmoColour;
		newSphere->SetLife(GizmoManager::GetInstance().gizmoLife);
		newSphere->vector[0] = position;
		newSphere->value[0] = radius;

		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoSphere->QueueOperation(newSphere);
	}

	void GizmoManager::DrawLine(Vector3 from, Vector3 to)
	{
		GizmoOperation::Ptr newLine = GizmoOperation::Create();

		newLine->colour = GizmoManager::GetInstance().gizmoColour;
		newLine->SetLife(GizmoManager::GetInstance().gizmoLife);
		newLine->vector[0] = from;
		newLine->vector[1] = to;

		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoLine->QueueOperation(newLine);
	}

	void GizmoManager::DrawAxisMatrix(Matrix4 mat)
	{
		GizmoOperation::Ptr newAxis = GizmoOperation::Create();

		newAxis->SetLife(GizmoManager::GetInstance().gizmoLife);

		newAxis->vector[0] = mat.GetTranslation();
		newAxis->vector[1] = mat.GetRotation() * Vector3::RIGHT;
		newAxis->vector[2] = mat.GetRotation() * Vector3::UP;
		newAxis->vector[3] = mat.GetRotation() * Vector3::FORWARD;

		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoAxis->QueueOperation(newAxis);
	}

	void GizmoManager::DrawAxisVectors(Vector3 pos, Vector3 right, Vector3 up, Vector3 forward)
	{
		GizmoOperation::Ptr newAxis = GizmoOperation::Create();

		newAxis->SetLife(GizmoManager::GetInstance().gizmoLife);
		newAxis->vector[0] = pos;
		newAxis->vector[1] = right;
		newAxis->vector[2] = up;
		newAxis->vector[3] = forward;

		// Disabled because GizmoManager is currently disabled
		//GizmoManager::GetInstance().gizmoAxis->QueueOperation(newAxis);
	}

}