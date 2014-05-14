
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

		//eventManager = &EventManager::GetInstance();

		scriptManager = &ScriptManager::GetInstance();
		scriptManager->Setup();
		
		renderManager = &RenderManager::GetInstance();
		renderManager->Setup();

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

		uiManager = &UIManager::GetInstance();
		uiManager->Setup();

		// Create and Register Gizmos
		gizmoManager = &GizmoManager::GetInstance();
		gizmoManager->Setup();

		audioManager = &AudioManager::GetInstance();
		audioManager->Setup();
        
#ifndef __APPLE__
        // No graphs on OSX due to the UI being OpenGL 2 and everything being sad :'(
		ConsoleWindow::Ptr consoleWindow = ConsoleWindow::Create();
		consoleWindow->Setup();
		uiManager->AddUIWindow(consoleWindow);

		DebugStatsOverlay::Ptr debugStatsOverlay = DebugStatsOverlay::Create();
		debugStatsOverlay->Setup();
		uiManager->AddUIOverlay(debugStatsOverlay);

		fpsGraph = debugStatsOverlay->CreateGraph("FPS");

		scriptGraph = debugStatsOverlay->CreateGraph("script");
		scriptGraph->SetColour(Colour::BLUE);

		sceneGraph = debugStatsOverlay->CreateGraph("scene");
		sceneGraph->SetColour(Colour::CYAN);

		renderGraph = debugStatsOverlay->CreateGraph("render");
		renderGraph->SetColour(Colour::GREEN);

		eventsGraph = debugStatsOverlay->CreateGraph("events");
		eventsGraph->SetColour(Colour::ORANGE);
#endif
		sceneManager = &SceneManager::GetInstance();
		sceneManager->Setup();

		renderManager->SetSceneManager(sceneManager);
		renderManager->SetUIManager(uiManager);

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
				scriptGraph->AddValue(scriptClock.getElapsedTime().asMilliseconds());
			});

			/*
			sceneClock.restart();
			taskGroup.run([&]() {				
				sceneManager->Update(el);
				sceneManager->Cull();
				sceneGraph->AddValue(sceneClock.getElapsedTime().asMilliseconds());
			});
			*/
			
			taskGroup.run([&]() {
				gizmoManager->Update(el);
				sceneGraph->AddValue(sceneClock.getElapsedTime().asMilliseconds());
			});

			taskGroup.run([&]() {

				// Check Resources for changes
				resourceManager->Update(el);
			});

			taskGroup.wait();

			//taskGroup.run( [&]() { uiManager->OnUpdate(el); } );
		}
		else
		{
			//EventManager::ProcessEvents(0.f);
            resourceManager->Update(el);
			scriptManager->Update(el);
			sceneManager->Update(el);
			sceneManager->Cull();
		}
		resourceManager->Update(el);

		// Running the AudioManager after the ResourceManager until I can assess thread safety
		audioManager->Update(el);

		// The following managers currently don't support running in parallel due to OpenGL or whatever.
        gizmoManager->Update(el);

		// Tick the physics sim
		physicsManager->Update(el);

		sceneManager->Update(el);
		sceneManager->Cull();

		// UI cannot be updated in parallel due to SFGUI not being threadsafe, gfx access etc
		uiManager->OnUpdate(el);
		renderClock.restart();
		renderManager->Draw(el);
        if ( renderGraph )
        {
            renderGraph->AddValue(renderClock.getElapsedTime().asMilliseconds());
        }
	}
    
    void EpsilonManager::OnClose(void)
    {
        // Stop Scripts
		scriptManager->Destroy();
        
        // Clean up OpenGL
        renderManager->Destroy();
        
        // Stop UI
        uiManager->Destroy();

		physicsManager->Destroy();
        
        renderManager->CloseWindow();
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

			while (renderManager->PollEvent(event))
			{
				// Let the RenderManager process window event first.
				renderManager->ProcessEvent(event);

				// If the window is in focus
				if (renderManager->WindowInFocus())
				{
					// Process the UI events
					uiManager->ProcessEvent(event);

					// Process the input events AFTER UI so that UI gets preference
					inputManager->ProcessEvent(event);
				}

				if (event.type == sf::Event::Closed)
				{
                    // Break Main loop
                    hasClosed = true;
					break;
				}
			}
            
            // If a close is triggered, exit the loop.
            if (hasClosed)
            {
                break;
            }
			
			float el = clock.getElapsedTime().asMilliseconds();
			
            if ( fpsGraph )
            {
                fpsGraph->AddValue(el);
            }

			// Restart timing for next sequence
			clock.restart();

			OnUpdate(el / 1000.0f);
		}
        
        OnClose();
	}
}