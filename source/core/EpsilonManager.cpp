
#include <time.h>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <tbb/tbb.h>

#include "core/EpsilonManager.h"
#include "math/Defines.h"

namespace epsilon
{
	EpsilonManager::EpsilonManager(void) : threadEnable(true)
	{
	}

	EpsilonManager::~EpsilonManager(void)
	{
	}
    
    void EpsilonManager::SetBasePath(std::string bp)
    {
        basepath = bp;
    }

	void EpsilonManager::SetThreadEnable(bool enableThreads)
	{
		threadEnable = enableThreads;
	}

	void EpsilonManager::Setup(void)
	{
        logger = &Logger::getInstance();
        
		Log("Initialising Epsilon Manager");
		resourceManager = &ResourceManager::GetInstance();
        if (basepath != "")
        {
            resourceManager->SetBasePath(basepath);
        }
		resourceManager->BuildResourceInfo();
		
		renderManager = &RenderManager::GetInstance();
		renderManager->Setup();

		//eventManager = &EventManager::GetInstance();

		scriptManager = &ScriptManager::GetInstance();
		scriptManager->SetScriptsFolderPath("scripts/");
		scriptManager->SetCoreScript("core/main.py");
		scriptManager->Setup();
		
		shaderManager = &ShaderManager::GetInstance();
        shaderManager->Setup();

		textureManager = &TextureManager::GetInstance();
		textureManager->Setup();

		materialManager = &MaterialManager::GetInstance();
		materialManager->Setup();

		meshManager = &MeshManager::GetInstance();
		meshManager->Setup();

		inputManager = &InputManager::GetInstance();

		physicsManager = &PhysicsManager::GetInstance();
		physicsManager->Setup();
		
		audioManager = &AudioManager::GetInstance();
		audioManager->Setup();
        
		sceneManager = &SceneManager::GetInstance();
		sceneManager->Setup();

		renderManager->SetSceneManager(sceneManager);

		// Initialise the Python Engine code after all of the c++ managers
		// have initialised
		scriptManager->StartEngineCore();

		Log("Finished Setup");
	}

	void EpsilonManager::OnFrameStart()
	{
		inputManager->OnFrameStart();
		materialManager->OnFrameStart();
	}

	void EpsilonManager::OnUpdate(float el)
	{
#ifdef __APPLE__
		threadEnable = false;
#endif
		if (threadEnable)
		{
			tbb::task_group taskGroup;

			/*eventsClock.restart();
			taskGroup.run([&]() {
				EventManager::ProcessEvents(0.f);
				eventsGraph->AddValue(eventsClock.getElapsedTime().asMilliseconds());
			});*/

			scriptClock.restart();
			taskGroup.run([&]() {
				scriptManager->Update(el);
				//scriptGraph->AddValue(scriptClock.getElapsedTime().asMilliseconds());
			});

			/*
			sceneClock.restart();
			taskGroup.run([&]() {				
				sceneManager->Update(el);
				sceneManager->Cull();
				sceneGraph->AddValue(sceneClock.getElapsedTime().asMilliseconds());
			});
			*/
			
			//taskGroup.run([&]() {
			//	//gizmoManager->Update(el);
			//	//sceneGraph->AddValue(sceneClock.getElapsedTime().asMilliseconds());
			//});

			taskGroup.run([&]() {

				// Check Resources for changes
				resourceManager->Update(el);
			});

			taskGroup.wait();

			//taskGroup.run( [&]() { uiManager->OnUpdate(el); } );
            resourceManager->Update(el);
        }
		else
		{
			//EventManager::ProcessEvents(0.f);
            resourceManager->Update(el);

			sceneManager->Update(el);
			sceneManager->Cull();
            
            scriptManager->Update(el);
		}
		
		// Running the AudioManager after the ResourceManager until I can assess thread safety
		audioManager->Update(el);

		// The following managers currently don't support running in parallel due to OpenGL or whatever.
        
        renderManager->OnUpdate(el);

		// Tick the physics sim
		physicsManager->Update(el);

		sceneManager->Update(el);
		sceneManager->Cull();

		renderClock.restart();
		renderManager->Draw(el);
	}
    
    void EpsilonManager::OnClose(void)
    {
        // Stop Scripts
		scriptManager->Destroy();
        
        // Clean up OpenGL and UI
        renderManager->Destroy();
        
		// Clean up Physics
		physicsManager->Destroy();
    }

	void EpsilonManager::Run(void)
	{
		// Run Loop

		// Handle events here until event manager is implemented.
		sf::Event event;
		sf::Clock clock;
        bool hasClosed = false;

		while ( renderManager->WindowOpen() )
		{
			// Fire On Frame Start Events
			OnFrameStart();

			// Get a queued event
			while (renderManager->PollEvent(event))
			{
				// Pass it through for input processing
				inputManager->ProcessEvent(event);
			}

			// If a quit has been detected
			if ( !renderManager->IsRunning() )
			{
				// Exit Game loop
				break;
			}
			float el = clock.getElapsedTime().asMilliseconds();
			
			// Restart timing for next sequence
			clock.restart();

			OnUpdate(el / 1000.0f);
		}
        
        OnClose();
	}
}