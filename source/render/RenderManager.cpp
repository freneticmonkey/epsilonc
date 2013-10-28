#include "render/RenderManager.h"

namespace epsilon
{
	RenderManager::RenderManager(void)
	{
	}


	RenderManager::~RenderManager(void)
	{
		if ( fpsText ) { delete fpsText; }
		if ( font ) { delete font; }
		if ( window ) { delete window; }
	}

	void RenderManager::Setup(void)
	{
		using namespace sf;

		Log("Initialising RenderManager");

		window = new RenderWindow( VideoMode(800, 600),
								   "Epsilon Engine",
								   Style::Default,
								   ContextSettings(32));
		font = new Font();
		if (!font->loadFromFile("resources/sansation.ttf"))
		{
			Log("Unable to find font: sansation.ttf");
		}
		else
		{
			fpsText = new Text("FPS: 0.0", *font);
			fpsText->setColor(Color(255,0,0,170));
			fpsText->setPosition(250.f, 450.f);
		}

		//window->setVerticalSyncEnabled(true);

		// Initialising OpenGL
		GLenum GlewInitResult;

		GlewInitResult = glewInit();

		if (GLEW_OK != GlewInitResult) 
		{
			Log("ERROR: Error initialising GLEW");
			Log((const char *)glewGetErrorString(GlewInitResult));
		}
	
		Log("INFO: OpenGL Version: ");
		Log((const char *)glGetString(GL_VERSION));

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);

		fps = 0.0f;

	}

	//void RenderManager::Draw(sf::Time el)
	void RenderManager::Draw(float el)
	{
		
		// Make this window active
		window->setActive(true);

		// Clear the window
		window->clear(sf::Color(50,50,50,255));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//
		// Do OpenGL drawing here.
		if (sceneManager)
		{
			sceneManager->Draw();
		}
		window->pushGLStates();
		window->resetGLStates();

		if ( fpsText )
		{
			// Calculate raw FPS
			//sf::Int64 fps = 1000000 / el.asMicroseconds();
			fps = (fps * 0.1f) + ((1.0f / el) * 0.9f);
			// If obviously broken FPS reset
			if (fps > 100000.0f )
			{
				fps = (1.0f / el);
			}
			std::ostringstream sstr;
			sstr << fps;
			std::string output("FPS: ");
			output += std::string(sstr.str());

			fpsText->setString(output);
			window->setTitle(output);

			window->draw(*fpsText);
		}
		window->popGLStates();
		
		// Draw the GUI
		if ( uiManager )
		{
			uiManager->Draw(window);
		}
		

		// Display the frame
		window->display();
	}

	bool RenderManager::WindowOpen(void)
	{
		if (window)
		{
			return window->isOpen();
		}
		else
		{
			return false;
		}
	}

	void RenderManager::CloseWindow(void)
	{
		if (window)
		{
			window->close();
		}
	}

	bool RenderManager::PollEvent(sf::Event &event)
	{
		return window->pollEvent(event);
	}

	void RenderManager::SetSceneManager(SceneManager * sm)
	{
		sceneManager = sm;
	}
    
	void RenderManager::SetUIManager(UIManager * uim)
	{
		if ( uim )
		{
			uiManager = uim;
		}
	}
}
