#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "resource/ResourceManager.h"
#include "audio/AudioManager.h"
#include "core/InputManager.h"
#include "events/EventManager.h"
#include "render/BGFXRenderManager.h"
#include "render/material/ShaderManager.h"
#include "render/material/MaterialManager.h"
#include "render/mesh/MeshManager.h"
#include "render/texture/TextureManager.h"
#include "scene/SceneManager.h"
#include "script/ScriptManager.h"
#include "physics/PhysicsManager.h"

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
		//EventManager * eventManager;
		InputManager * inputManager;
		AudioManager * audioManager;
		ScriptManager * scriptManager;
		BGFXRenderManager * renderManager;
        ShaderManager * shaderManager;
		MaterialManager * materialManager;
		MeshManager * meshManager;
		TextureManager * textureManager;
		UIManager * uiManager;
		SceneManager * sceneManager;
		PhysicsManager * physicsManager;
		CycleValue cycle;
		Transform::Ptr triangleTrans;
		Camera::Ptr camera;

		sf::Clock  scriptClock;
		sf::Clock  sceneClock;
		sf::Clock  renderClock;
		sf::Clock  eventsClock;

		bool	   threadEnable;
        
        std::string basepath;
	};
}

