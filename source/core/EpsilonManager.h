#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "resource/ResourceManager.h"
#include "events/EventManager.h"
#include "core/InputManager.h"
#include "render/RenderManager.h"
#include "render/material/ShaderManager.h"
#include "render/material/MaterialManager.h"
#include "render/texture/TextureManager.h"
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
        
        void SetBasePath(std::string bp);
        
		void Setup(void);
		void OnFrameStart(void);
		void OnUpdate(float el);
        void OnClose(void);
		void Run(void);

		void MeshTest(void);

		void SetThreadEnable(bool enableThreads);

	private:
        Logger * logger;
		ResourceManager * resourceManager;
		EventManager * eventManager;
		InputManager * inputManager;
		ScriptManager * scriptManager;
		RenderManager * renderManager;
        ShaderManager * shaderManager;
		MaterialManager * materialManager;
		TextureManager * textureManager;
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
        
        std::string basepath;
	};
}

