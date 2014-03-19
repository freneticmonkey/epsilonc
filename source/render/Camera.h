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
		typedef std::shared_ptr<Camera> Ptr;

		static Camera::Ptr Create();
		static Camera::Ptr Create(std::string name);

		explicit Camera(const private_struct &);
		Camera(const private_struct &, std::string name);
		~Camera(void) {}

		void LookAt(Vector3 target);
		void LookAt(Vector3 from, Vector3 to);
		void LookAt(float x, float y, float z, 
					float lookAtX, float lookAtY, float lookAtZ);

		void FPS(Vector3 pos, float pitch, float yaw);
		
		Matrix4 GetProjectionMatrix() { return projMatrix; }
		Matrix4 GetViewMatrix() { return viewMatrix; }

		Vector3 ScreenToWorldCoordinate(Vector2 screenPos);
		Vector2 WorldToScreenCoordinate(Vector3 worldPos);

	private:
		void Setup();
		void BuildProjectionMatrix(float fov, float ratio, float nearP, float farP);

		Matrix4 viewMatrix;
		Matrix4 projMatrix;
		float	nearDist;
		float	farDist;
		float	fov;
		float	ratio;
		int		width;
		int		height;
	};
}