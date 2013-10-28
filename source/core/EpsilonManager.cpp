
#include <time.h>

#include "core/EpsilonManager.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include "render/MeshFactory.h"

namespace epsilon
{
	EpsilonManager::EpsilonManager(void)
	{
	}


	EpsilonManager::~EpsilonManager(void)
	{
		if ( sceneManager ) { delete sceneManager; }
		if ( uiManager ) { delete uiManager; }
		if ( renderManager ) { delete renderManager; }
	}

	void EpsilonManager::Setup(void)
	{
		Log("Initialising Epsilon Manager");

		renderManager = new RenderManager();
		renderManager->Setup();

		uiManager = new UIManager();
		uiManager->Setup();
		
		ConsoleWindow::Ptr consoleWindow = ConsoleWindow::Create();
		consoleWindow->Setup();
		uiManager->AddUIWindow(consoleWindow);

		sceneManager = new SceneManager();
		sceneManager->Setup();

		renderManager->SetSceneManager(sceneManager);
		renderManager->SetUIManager(uiManager);

		MeshTest();

		Log("Finished Setup");

		cycle.SetFrequency(100);

		camera = sceneManager->CurrentScene()->GetActiveCamera();
		camera->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -5.f);
		camera->LookAt(Vector3());
	}

	void EpsilonManager::MeshTest(void)
	{
		//sceneManager->CurrentScene()->Root()->Renderer()->GetMesh()->SetMeshData(verts, norms, tc, inds);
		//sceneManager->CurrentScene()->Root()->Renderer()->GetMesh()->SetMeshData(verts, norms, colours, tc, inds);
		
		//sceneManager->CurrentScene()->Root()->Renderer()->SetMesh(MeshFactory::GenerateSphere(8,8));
		//sceneManager->CurrentScene()->Root()->Transform()->setPosition(0.f, 10.f, -10.f);
		
		/*
		Node::Ptr triangle = sceneManager->CurrentScene()->Root()->CreateChildNode();
		triangle->Renderer()->SetMesh(MeshFactory::GenerateTriangle());
		triangle->Transform()->setPosition(0.f, 0.f, 0.f);
		*/
		
		Node::Ptr triangle2 = sceneManager->CurrentScene()->Root()->CreateChildNode();
		triangle2->SetName("triangle");
		triangle2->AddComponent(Renderer::Create());
		triangle2->GetComponent<Renderer>()
				 ->SetMesh(MeshFactory::GenerateTriangle());
				 //->SetMesh(MeshFactory::GenerateSphere());
		triangleTrans = triangle2->GetComponent<Transform>();
		triangleTrans->SetPosition(0.f, 0.f, 0.f);
			//.Rotate(Vector3::UNIT_Y, 45.0f);
		
		// Add a sub child
		Node::Ptr tChild = triangle2->CreateChildNode();
		tChild->AddComponent(Renderer::Create());
		tChild->GetComponent<Renderer>()
			  ->SetMesh(MeshFactory::GenerateSphere());
		tChild->GetComponent<Transform>()
			  ->SetPosition(1.0f, 0.0f, 0.0f);
		
		Node::Ptr plane = sceneManager->CurrentScene()->Root()->CreateChildNode();
		plane->SetName("plane");

		plane->AddComponent(Renderer::Create());
		plane->GetComponent<Renderer>()
			  ->SetMesh(MeshFactory::GeneratePlane());
		plane->GetComponent<Transform>()
			  ->SetPosition(0.0f, -2.0f, 0.0f);

		/*
		Node::Ptr sphere = sceneManager->CurrentScene()->Root()->CreateChildNode();
		sphere->Renderer()->SetMesh(MeshFactory::GenerateSphere(8,8));
		sphere->Transform()->setPosition(0.f, 0.f, 10.f);
		*/
		
	}

	//void EpsilonManager::OnUpdate(sf::Time el)
	void EpsilonManager::OnUpdate(float el)
	{
		// Create a cyclical value that cycles 100 times/second
		float value = cycle.GetValue(el);
		//triangleTrans->SetPosition(0.f, 0.0f, value);
		//triangleTrans->Rotate(Vector3::UP, 1.0f * (value/100.0));

		uiManager->OnUpdate(el);
		sceneManager->Cull();
		renderManager->Draw(el);
	}

	void EpsilonManager::Run(void)
	{
		// Run Loop

		// Handle events here until event manager is implemented.
		sf::Event event;
		//sf::Clock clock;
		clock_t lastTime = clock();
		float el = 0.0f;

		while ( renderManager->WindowOpen() )
		{
			//clock.restart();
			//sleep(Time(milliseconds(1)));

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
					//triangleTrans->Translate(trans);
					//aLog(std::to_string(triangleTrans->GetPosition().z));
					Vector3 la = camTrans->GetPosition() + (Vector3(0, 0, 5.0f));
					camera->LookAt(Vector3());
				}

			}
			//OnUpdate(clock.getElapsedTime());
			clock_t currTime = clock();
			el = diffclock(currTime, lastTime) / 1000.0f;
			OnUpdate(el);
			lastTime = currTime;
		}
	}
}