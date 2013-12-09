#pragma once
#include "EpsilonCore.h"

#include "scene/Node.h"

namespace epsilon
{
	class Camera : public Node
	{
	protected:
		struct private_struct {};

	public:
		typedef shared_ptr<Camera> Ptr;

		static Camera::Ptr Create();
		static Camera::Ptr Create(string name);

		explicit Camera(const private_struct &);
		Camera(const private_struct &, string name);
		~Camera(void) {}

		void LookAt(Vector3 target);
		void LookAt(Vector3 from, Vector3 to);
		void LookAt(float x, float y, float z, 
					float lookAtX, float lookAtY, float lookAtZ);

		Matrix4 GetProjectionMatrix() { return projMatrix; }
		Matrix4 GetViewMatrix() { return viewMatrix; }

	private:
		void Setup();
		void BuildProjectionMatrix(float fov, float ratio, float nearP, float farP);

		Matrix4 viewMatrix;
		Matrix4 projMatrix;
		float	nearDist;
		float	farDist;
		float	fov;
		float	ratio;
	};
}