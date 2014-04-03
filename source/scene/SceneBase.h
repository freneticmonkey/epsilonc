#pragma once

#include "render/Camera.h"
#include "render/Light.h"


namespace epsilon
{

	class SceneBase
	{
	public:
		typedef std::shared_ptr<SceneBase> Ptr;
		
		// Default constructors.

		virtual bool AddCamera(Camera::Ptr newCamera) = 0;
		virtual bool AddLight(Light::Ptr newLight) = 0;
	};
}