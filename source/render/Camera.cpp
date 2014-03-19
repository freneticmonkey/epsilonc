#include "render/Camera.h"
#include "math/Defines.h"
#include "math/Matrix.h"

namespace epsilon
{
	Camera::Ptr Camera::Create()
	{
		Camera::Ptr newCamera = std::make_shared<Camera>(private_struct());
		newCamera->CreateComponents();
		return newCamera;
	}

	Camera::Ptr Camera::Create(std::string name)
	{
		Camera::Ptr newCamera = std::make_shared<Camera>(private_struct(), name);
		newCamera->CreateComponents();
		return newCamera;
	}

	Camera::Camera(const private_struct &) : Node(Node::private_struct()),
											 nearDist(0.001f),
											 farDist(1000.f),
											 fov(45.0f),
											 width(800),
											 height(600)
	{
		Setup();
	}

	Camera::Camera(const private_struct &, std::string name) : Node(Node::private_struct(), name),
																nearDist(0.001f),
																farDist(1000.f),
																fov(45.0f),
																width(800),
																height(600)
	{
		Setup();
	}

	void Camera::Setup()
	{
		ratio = (1.0f * width) / height;

		BuildProjectionMatrix(fov, ratio, nearDist, farDist);
	}

	void Camera::LookAt(Vector3 target)
	{
		Vector3 from = GetComponent<Transform>()->GetPosition();
		LookAt(from, target);
	}

	void Camera::LookAt(Vector3 from, Vector3 to)
	{
		LookAt(from.x, from.y, from.z, to.x, to.y, to.z );
	}

	void Camera::LookAt(float x, float y, float z, 
						float lookAtX, float lookAtY, float lookAtZ)
	{
        Vector3 up(0.f, 1.f, 0.f);
        Vector3 from(x, y, z);
        Vector3 to(lookAtX, lookAtY, lookAtZ);
        
		
		// Invert the y-axis - cause opengl.
		//from.y = -from.y;
		// create the look at matrix.
		viewMatrix = Matrix4::CreateLookAt(from, to, up);

		// Set the orientation in the transform from the look at matrix
		GetComponent<Transform>()->SetLocalOrientation(viewMatrix.GetRotation());

		GetComponent<Transform>()->SetPosition(from);
	}

	void Camera::FPS(Vector3 pos, float pitch, float yaw)
	{
		float cosPitch = cos(pitch);
		float sinPitch = sin(pitch);
		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);

		Vector3 xaxis(cosYaw, 0, -sinYaw);
		Vector3 yaxis(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
		Vector3 zaxis(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

		viewMatrix = Matrix4(
			xaxis.x,		yaxis.x,		  zaxis.x, 0,
			xaxis.y,		yaxis.y,		  zaxis.y, 0,
			xaxis.z,		yaxis.z,		  zaxis.z, 0,
			-xaxis.Dot(pos), -yaxis.Dot(pos), -zaxis.Dot(pos), 1
		);
		
		viewMatrix.Transpose();

		GetComponent<Transform>()->SetPosition(viewMatrix.GetTranslation());

		//// Set the orientation in the transform from the look at matrix
		GetComponent<Transform>()->SetLocalOrientation(viewMatrix.GetRotation());
	}

	Vector3 Camera::ScreenToWorldCoordinate(Vector2 screenPos)
	{
		float x = 2.0 * screenPos.x / width - 1;
		float y = -2.0 * screenPos.y / height + 1;
		Matrix4 viewProj = projMatrix * viewMatrix;
		Vector3 v = Vector3(x, y, 0) * viewProj.Inverse();
		return v;
	}

	Vector2 Camera::WorldToScreenCoordinate(Vector3 worldPos)
	{
		Matrix4 viewProj = projMatrix * viewMatrix;
		// transform world point to clipping coordinates
		worldPos = viewProj * worldPos;
		int screenX = (int)round((worldPos.x + 1) / 2.0) * width;
		// -Y because screen Y axis is top down
		int screenY = (int)round((1 - worldPos.y) / 2.0) * height;
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
		/*Matrix4 invertY;
		invertY[5] = -1;
		projMatrix *= invertY;*/
		projMatrix.Scale(1.0f, -1.f, 1.0f);
	}

}

