#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "render/RenderManager.h"
#include "scene/SceneManager.h"
#include "script/ScriptManager.h"
#include "ui/UIManager.h"
#include "ui/ConsoleWindow.h"

#include "utilities/Utilities.h"

namespace epsilon
{
	class EpsilonManager
	{
	public:
		EpsilonManager(void);
		~EpsilonManager(void);

		void Setup(void);
		//void OnUpdate(sf::Time el);
		void OnUpdate(float el);
		void Run(void);

		void MeshTest(void);

	private:
		ScriptManager * scriptManager;
		RenderManager * renderManager;
		UIManager * uiManager;
		SceneManager * sceneManager;

		CycleValue cycle;
		Transform::Ptr triangleTrans;
		Camera::Ptr camera;
	};
}

