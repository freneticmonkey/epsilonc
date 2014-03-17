#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "events/EventManager.h"
#include "core/InputManager.h"
#include "render/RenderManager.h"
#include "render/gizmos/GizmoManager.h"
#include "scene/SceneManager.h"
#include "script/ScriptManager.h"
#include "ui/UIManager.h"
#include "ui/ConsoleWindow.h"
#include "ui/DebugStatsOverlay.h"
#include "ui/Graph.h"

#include "utilities/Utilities.h"

namespace epsilon
{
	class EpsilonManager
	{
	public:
		EpsilonManager(void);
		~EpsilonManager(void);

		void Setup(void);
		void OnUpdate(float el);
		void Run(void);

		void MeshTest(void);

		void SetThreadEnable(bool enableThreads);

	private:
		EventManager * eventManager;
		InputManager * inputManager;
		ScriptManager * scriptManager;
		RenderManager * renderManager;
		UIManager * uiManager;
		SceneManager * sceneManager;
		GizmoManager * gizmoManager;

		CycleValue cycle;
		Transform::Ptr triangleTrans;
		Camera::Ptr camera;

		Graph::Ptr fpsGraph;
		Graph::Ptr scriptGraph;
		Graph::Ptr sceneGraph;
		Graph::Ptr renderGraph;
		Graph::Ptr eventsGraph;

		sf::Clock  scriptClock;
		sf::Clock  sceneClock;
		sf::Clock  renderClock;
		sf::Clock  eventsClock;

		bool	   threadEnable;
	};
}

