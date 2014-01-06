#include "render/RenderManager.h"

namespace epsilon
{
	RenderManager::RenderManager(void) : currFPSSample(0)
	{
		for ( int i = 0; i < NUM_FPS_SAMPLES; i++)
		{
			fpsSamples[i] = 0.0f;
		}

		// Create the Render State Stack
		stateStack = RenderStateStack::Create();
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
        
        ContextSettings openglSettings;
        openglSettings.depthBits = 32;
        openglSettings.stencilBits = 8;
        openglSettings.antialiasingLevel = 0;
        openglSettings.majorVersion = 2;
        openglSettings.minorVersion = 1;
		window = new RenderWindow( VideoMode(800, 600),
								   "Epsilon Engine",
								   Style::Default,
								   openglSettings);
		font = new Font();
		//if (!font->loadFromFile("/Users/scottporter/Development/Projects/C++/epsilonc/resources/sansation.ttf"))
		if (!font->loadFromFile("resources/sansation.ttf"))
		{
			Log("Unable to find font: sansation.ttf");
		}
		else
		{
			// TODO: This needs to be moved into a UI Window with other debug info.
			fpsText = new Text("FPS: 0.0", *font);
			fpsText->setColor(Color(255,255,255,170));
			fpsText->setPosition(700.f, 0.f);
			fpsText->setCharacterSize(16);
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

		// Reset any render states
		stateStack->Reset();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        CheckOpenGLError("Clear Error Before Draw");

		// Do OpenGL drawing here.
		if (sceneManager)
		{
			sceneManager->Draw(stateStack);
		}
		window->pushGLStates();
		window->resetGLStates();

		if ( fpsText )
		{
			std::ostringstream sstr;
			sstr << GetFPS(el);
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
		return ( window ) ? window->isOpen() : false;
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

	float RenderManager::GetFPS(float el)
	{
		fpsSamples[currFPSSample % NUM_FPS_SAMPLES] = 1.0f / el;
		currFPSSample++;

		float fps = 0.0f;

		for ( int i = 0; i < NUM_FPS_SAMPLES; i++ )
		{
			fps += fpsSamples[i];
		}
		fps /= NUM_FPS_SAMPLES;
		return fps;
	}
}
