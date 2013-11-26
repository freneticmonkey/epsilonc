
#include <time.h>

#include "core/EpsilonManager.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include "render/MeshFactory.h"
#include "math/Defines.h"

namespace epsilon
{
	EpsilonManager::EpsilonManager(void)
	{
	}

	EpsilonManager::~EpsilonManager(void)
	{
		if ( uiManager ) { delete uiManager; }
	}

	void EpsilonManager::Setup(void)
	{
		Log("Initialising Epsilon Manager");
		eventManager = &EventManager::GetInstance();

		scriptManager = &ScriptManager::GetInstance();
		scriptManager->Setup();
		scriptManager->StartEngineCore();

		renderManager = &RenderManager::GetInstance();
		renderManager->Setup();

		uiManager = new UIManager();
		uiManager->Setup();
		
		ConsoleWindow::Ptr consoleWindow = ConsoleWindow::Create();
		consoleWindow->Setup();
		uiManager->AddUIWindow(consoleWindow);

		sceneManager = &SceneManager::GetInstance();
		sceneManager->Setup();

		renderManager->SetSceneManager(sceneManager);
		renderManager->SetUIManager(uiManager);

		MeshTest();

		Log("Finished Setup");

		cycle.SetFrequency(100);

		camera = sceneManager->CurrentScene()->GetActiveCamera();
		camera->GetComponent<Transform>()->SetPosition(0.0f, 1.0f, -8.f);
		camera->LookAt(Vector3());
	}

	void EpsilonManager::MeshTest(void)
	{
		//Node::Ptr triangle2 = sceneManager->CurrentScene()->Root()->CreateChildNode();
		//triangle2->SetName("triangle");
		//triangle2->AddComponent(Renderer::Create());
		//triangle2->GetComponent<Renderer>()
		//		 ->SetMesh(MeshFactory::GenerateTriangle());
		//triangleTrans = triangle2->GetComponent<Transform>();
		//triangleTrans->SetPosition(0.f, 0.f, 0.f);
		//
		//// Add a sub child
		Node::Ptr tChild = sceneManager->CurrentScene()->Root()->CreateChildNode();
		tChild->SetName("sphere");
		tChild->AddComponent(Renderer::Create());
		tChild->GetComponent<Renderer>()
			  ->SetMesh(MeshFactory::GenerateSphere());
		tChild->GetComponent<Transform>()
			  ->SetPosition(1.0f, 1.0f, 0.0f);
		tChild->AddComponent(scriptManager->CreateBehaviour("MyBehaviourClass.py"));
		//
		Node::Ptr plane = sceneManager->CurrentScene()->Root()->CreateChildNode();
		plane->SetName("plane");

		plane->AddComponent(Renderer::Create());
		plane->GetComponent<Renderer>()
			  ->SetMesh(MeshFactory::GeneratePlane(4, 4));
		
		Node::Ptr scriptsRoot = sceneManager->CurrentScene()->Root()->CreateChildNode();
		scriptsRoot->SetName("scriptRoot");

		Node::Ptr script2 = scriptsRoot->CreateChildNode();
		script2->AddComponent(scriptManager->CreateBehaviour("TestingFeatures.py"));

		// Create a Grid
		if ( true )
		{
			Node::Ptr grid = sceneManager->CurrentScene()->Root()->CreateChildNode();
			grid->SetName("grid");

			grid->AddComponent(Renderer::Create());
			grid->GetComponent<Renderer>()
				->SetMesh(MeshFactory::GenerateGrid(10, 1));
		}
	}

	//void EpsilonManager::OnUpdate(sf::Time el)
	void EpsilonManager::OnUpdate(float el)
	{
		EventManager::ProcessEvents(0.f);
		scriptManager->Update(el);
		uiManager->OnUpdate(el);
		sceneManager->Cull();
		renderManager->Draw(el);
	}

	void EpsilonManager::Run(void)
	{
		// Run Loop

		// Handle events here until event manager is implemented.
		sf::Event event;
		sf::Clock clock;

		while ( renderManager->WindowOpen() )
		{
			while ( renderManager->PollEvent( event ) )
			{
				uiManager->ProcessEvent(event);

				if ( event.type == sf::Event::Closed )
				{
					renderManager->CloseWindow();
				}

				if ( event.type == sf::Event::KeyPressed )
				{

					if ( event.key.code == sf::Keyboard::Escape )
					{
						renderManager->CloseWindow();
					}
				}

				if ( event.type == sf::Event::KeyReleased )
				{
					Transform::Ptr camTrans = camera->GetComponent<Transform>();
					float amount = 1.0f;// * el;
					Vector3 trans;
					switch ( event.key.code )
					{
					case sf::Keyboard::A:
						trans.x = -amount;
						break;
					case sf::Keyboard::D:
						trans.x = amount;
						break;
					case sf::Keyboard::W:
						trans.z = amount;
						break;
					case sf::Keyboard::S:
						trans.z = -amount;
						break;

					case sf::Keyboard::R:
						trans.y = amount;
						break;
					case sf::Keyboard::F:
						trans.y = -amount;
						break;
					}
					camTrans->Translate(trans);
					Vector3 la = camTrans->GetPosition() + (Vector3(0, 0, 5.0f));
					camera->LookAt(Vector3());
				}

			}
			float el = clock.getElapsedTime().asMicroseconds() / 1000000.0f;
			// Restart timing for next sequence
			clock.restart();

			OnUpdate(el);
			

			
		}
	}
}