#include "render/Camera.h"
#include "math/Defines.h"
#include "math/Matrix.h"

#include "glm/glm.hpp"

namespace epsilon
{
	Camera::Ptr Camera::Create()
	{
		Camera::Ptr newCamera = make_shared<Camera>(private_struct());
		newCamera->CreateComponents();
		return newCamera;
	}

	Camera::Ptr Camera::Create(string name)
	{
		Camera::Ptr newCamera = make_shared<Camera>(private_struct(), name);
		newCamera->CreateComponents();
		return newCamera;
	}

	Camera::Camera(const private_struct &) : Node(Node::private_struct())
	{
		Setup();
	}

	Camera::Camera(const private_struct &, string name) : Node(Node::private_struct(), name)
	{
		Setup();
	}

	void Camera::Setup()
	{
		nearDist = 0.000001f;
		farDist = 100.f;
		fov = 45.0f;
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
        Vector3 to(lookAtX, lookAtY, lookAtZ);
        
        viewMatrix = Matrix4::CreateLookAt(from, to, up);
        		
        /*
        Vector3 dir(lookAtX - x, lookAtY - y, lookAtZ - z);
		Vector3 right = dir.Cross(up);
		Matrix4 aux;
		float viewM[16];
		float auxf[16];

		right.Normalise();
		up = right.Cross(dir);
		up.Normalise();
		
		// Setup the view matrix
		viewM[0]  = right[0];
		viewM[4]  = right[1];
		viewM[8]  = right[2];
		viewM[12] = 0.0f;
 
		viewM[1]  = up[0];
		viewM[5]  = up[1];
		viewM[9]  = up[2];
		viewM[13] = 0.0f;
 
		viewM[2]  = -dir[0];
		viewM[6]  = -dir[1];
		viewM[10] = -dir[2];
		viewM[14] =  0.0f;
 
		viewM[3]  = 0.0f;
		viewM[7]  = 0.0f;
		viewM[11] = 0.0f;
		viewM[15] = 1.0f;
		
		viewMatrix = Matrix4(viewM);

		// Translate the view matrix to the camera position
		aux.MakeIdentity();

		auxf[12] = -x;
		auxf[13] = -y;
		auxf[14] = -z;

		viewMatrix = viewMatrix * aux;

		// Ensure that the Camera transform is up to date
		Transform()->setPosition(x, y, z);

		// Update rotation


		// Step 2. Put the three vectors into the matrix to bulid a basis rotation matrix
            
		// This step isnt necessary, but im adding it because often you would want to convert from matricies to quaternions instead of vectors to quaternions
            // If you want to skip this step, you can use the vector values directly in the quaternion setup below
            Matrix mBasis = new Matrix(vRight.X, vRight.Y, vRight.Z, 0.0f,
                                        vUp.X, vUp.Y, vUp.Z, 0.0f,
                                        vDirection.X, vDirection.Y, vDirection.Z, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f);
            
            // Step 3. Build a quaternion from the matrix
            Quaternion qrot = new Quaternion();
            qrot.W = (float)Math.Sqrt(1.0f + mBasis.M11 + mBasis.M22 + mBasis.M33) / 2.0f;
            double dfWScale = qrot.W * 4.0;
            qrot.X = (float)((mBasis.M32 - mBasis.M23) / dfWScale);
            qrot.Y = (float)((mBasis.M13 - mBasis.M31) / dfWScale);
            qrot.Z = (float)((mBasis.M21 - mBasis.M12) / dfWScale);

            

		//Transform()->setOrientation(Quaternion(
         */

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
	}

}

