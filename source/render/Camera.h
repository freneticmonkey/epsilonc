#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"
#include "math/Matrix.h"

#include "render/material/ShaderUniform.h"

namespace epsilon
{
	class Camera : public NodeComponent
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

		void OnSetParent();
		
		bool IsActive() { return active; }
		void SetActive(bool isActive) { active = isActive; }

		Matrix4 GetProjectionMatrix();
		Matrix4 GetViewMatrix();

		Vector3 ScreenToWorldCoordinate(Vector2 screenPos);
		Vector2 WorldToScreenCoordinate(Vector3 worldPos);

		void	Update();

	private:
		void Setup();
		void BuildProjectionMatrix(float fov, float ratio, float nearP, float farP);

		Matrix4 projMatrix;
		float	nearDist;
		float	farDist;
		float	fov;
		float	ratio;
		int		width;
		int		height;

		bool	active;

		Transform::Ptr transform;

		ShaderUniform::Ptr viewMatrixUnf;
		ShaderUniform::Ptr projMatrixUnf;
	};
}