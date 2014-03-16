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

	Camera::Camera(const private_struct &) : Node(Node::private_struct())
	{
		Setup();
	}

	Camera::Camera(const private_struct &, std::string name) : Node(Node::private_struct(), name)
	{
		Setup();
	}

	void Camera::Setup()
	{
		nearDist = 0.001f;
		farDist = 1000.f;
		fov = 45.0f;

		// FIXME: The Screen resolution shouldn't be hardcoded here.
		ratio = (1.0f * 800.0f) / 600.0f;

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
        Vector3 to(lookAtX, -lookAtY, lookAtZ);
        
		if (from != GetComponent<Transform>()->GetPosition())
		{
			GetComponent<Transform>()->SetPosition(from);
		}
		
		// Invert the y-axis - cause opengl.
		from.y = -from.y;
		// create the look at matrix.
		viewMatrix = Matrix4::CreateLookAt(from, to, up);

		// Set the orientation in the transform from the look at matrix
		GetComponent<Transform>()->SetOrientation(viewMatrix.GetRotation());
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

