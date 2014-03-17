
#include <time.h>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include <tbb/tbb.h>

#include "core/EpsilonManager.h"


#include "render/MeshFactory.h"
#include "math/Defines.h"

namespace epsilon
{
	EpsilonManager::EpsilonManager(void) : threadEnable(true)
	{
	}

	EpsilonManager::~EpsilonManager(void)
	{
	}

	void EpsilonManager::SetThreadEnable(bool enableThreads)
	{
		threadEnable = enableThreads;
	}

	void EpsilonManager::Setup(void)
	{
		Log("Initialising Epsilon Manager");
		eventManager = &EventManager::GetInstance();

		scriptManager = &ScriptManager::GetInstance();
		scriptManager->Setup();

		renderManager = &RenderManager::GetInstance();
		renderManager->Setup();

		inputManager = &InputManager::GetInstance();

		uiManager = &UIManager::GetInstance();
		uiManager->Setup();

		// Create and Register Gizmos
		gizmoManager = &GizmoManager::GetInstance();
		gizmoManager->Setup();
		
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

		//fpsGraph->SetColour(Colour::RED);

		sceneManager = &SceneManager::GetInstance();
		sceneManager->Setup();

		renderManager->SetSceneManager(sceneManager);
		renderManager->SetUIManager(uiManager);

		// Initialise the Python Engine code after all of the c++ managers
		// have initialised
		scriptManager->StartEngineCore();

		Log("Finished Setup");
	}

	void EpsilonManager::OnUpdate(float el)
	{
#ifdef __APPLE__
		threadEnable = false;
#endif
		if (threadEnable)
		{
			tbb::task_group taskGroup;

			eventsClock.restart();
			taskGroup.run([&]() {
				EventManager::ProcessEvents(0.f);
				eventsGraph->AddValue(eventsClock.getElapsedTime().asMilliseconds());
			});

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
			
			taskGroup.run([&]() {
				gizmoManager->Update(el);
				sceneGraph->AddValue(sceneClock.getElapsedTime().asMilliseconds());
			});
			*/

			taskGroup.wait();

			//taskGroup.run( [&]() { uiManager->OnUpdate(el); } );
		}
		else
		{
			EventManager::ProcessEvents(0.f);
			scriptManager->Update(el);
			sceneManager->Update(el);
			sceneManager->Cull();
		}
		gizmoManager->Update(el);

		sceneManager->Update(el);
		sceneManager->Cull();

		// UI cannot be updated in parallel due to SFGUI not being threadsafe, gfx access etc
		uiManager->OnUpdate(el);
		renderClock.restart();
		renderManager->Draw(el);
		renderGraph->AddValue(renderClock.getElapsedTime().asMilliseconds());
	}

	void EpsilonManager::Run(void)
	{
		// Run Loop

		// Handle events here until event manager is implemented.
		sf::Event event;
		sf::Clock clock;

		while ( renderManager->WindowOpen() )
		{
			// Fire On Frame Start Events
			inputManager->OnFrameStart();

			while ( renderManager->PollEvent( event ) )
			{
				// Process the UI events
				uiManager->ProcessEvent(event);

				// Process the input events AFTER UI so that UI gets preference
				inputManager->ProcessEvent(event);

				if ( event.type == sf::Event::Closed )
				{
					renderManager->CloseWindow();
				}

			}
			float el = clock.getElapsedTime().asMilliseconds();
			
			fpsGraph->AddValue(el);
			
			// Restart timing for next sequence
			clock.restart();

			OnUpdate(el / 1000.0f);
		}

		// Stop Scripts
		scriptManager->Destroy();

	}
}