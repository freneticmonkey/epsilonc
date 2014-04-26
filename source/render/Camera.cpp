#include "render/Camera.h"
#include "math/Defines.h"
#include "math/Matrix.h"
#include "scene/Transform.h"

#include "render/material/ShaderManager.h"

namespace epsilon
{
	Camera::Ptr Camera::Create()
	{
		Camera::Ptr newCamera = std::make_shared<Camera>(private_struct());
		return newCamera;
	}

	Camera::Ptr Camera::Create(std::string name)
	{
		Camera::Ptr newCamera = std::make_shared<Camera>(private_struct(), name);
		return newCamera;
	}

	Camera::Camera(const private_struct &) : nearDist(0.001f),
											 farDist(1000.f),
											 fov(45.0f),
											 width(800),
											 height(600),
											 NodeComponent("Camera")
	{
		Setup();
	}

	Camera::Camera(const private_struct &, std::string name) : nearDist(0.001f),
															   farDist(1000.f),
															   fov(45.0f),
															   width(800),
															   height(600),
															   NodeComponent(name, "Camera")
	{
		Setup();
	}

	void Camera::OnSetParent()
	{
		if (this->componentParent)
		{
			transform = this->componentParent->GetComponent<Transform>();
		}
		else
		{
			transform = nullptr;
		}
	}

	void Camera::Setup()
	{
		ratio = (float)((1.0f * width) / height);

		BuildProjectionMatrix(fov, ratio, nearDist, farDist);

		ShaderManager * sm = &ShaderManager::GetInstance();

		UniformBuffer::Ptr globalMatUnf = sm->GetUniformBuffer("GlobalMatrices");

		viewMatrixUnf = globalMatUnf->GetUniform("viewMatrix");
		projMatrixUnf = globalMatUnf->GetUniform("projectionMatrix");
	}
	
	Matrix4 Camera::GetProjectionMatrix()
	{ 
		return projMatrix; 
	}

	Matrix4 Camera::GetViewMatrix()
	{ 
		if (transform)
		{
			return transform->_getFullTransform();
		}
		else
		{
			return Matrix4();
		}
	}

	Vector3 Camera::ScreenToWorldCoordinate(Vector2 screenPos)
	{
		Matrix4 viewMatrix = GetParent()->GetComponent<Transform>()->_getFullTransform();
		float x = (float)2.0 * screenPos.x / width - 1;
		float y = (float)-2.0 * screenPos.y / height + 1;
		Matrix4 viewProj = projMatrix * viewMatrix;
		Vector3 v = Vector3(x, y, 0) * viewProj.Inverse();
		return v;
	}

	Vector2 Camera::WorldToScreenCoordinate(Vector3 worldPos)
	{
		Matrix4 viewMatrix = GetParent()->GetComponent<Transform>()->_getFullTransform();
		Matrix4 viewProj = projMatrix * viewMatrix;
		// transform world point to clipping coordinates
		worldPos = viewProj * worldPos;
		float screenX = (float)round((worldPos.x + 1) / 2.0) * width;
		// -Y because screen Y axis is top down
		float screenY = (float)round((1 - worldPos.y) / 2.0) * height;
		return Vector2(screenX, screenY);
	}

	void Camera::BuildProjectionMatrix(float fov, float ratio, float nearP, float farP)
	{
		float f = 1.0f / tan (fov * (PI / 360.0f));

		projMatrix[0] = f / ratio;
		projMatrix[1 * 4 + 1] = f;
		projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
		projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
		projMatrix[2 * 4 + 3] = -1.0f;
		projMatrix[3 * 4 + 3] = 0.0f;

		// Inverting the Y-axis cause OpenGL :/
		/*
		Matrix4 invertY;
		invertY[5] = -1;
		projMatrix *= invertY;
		*/
		//projMatrix.Scale(1.0f, 1.f, 1.0f);
	}

	void Camera::Update()
	{
		// Push matrices to the Uniform buffer
		if (viewMatrixUnf)
		{
			viewMatrixUnf->SetMatrix4(GetViewMatrix());
		}

		if (projMatrixUnf)
		{
			projMatrixUnf->SetMatrix4(projMatrix);
		}
	}

}

