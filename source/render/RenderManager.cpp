#include "render/RenderManager.h"
#include "render/gizmos/GizmoCube.h"

namespace epsilon
{
	RenderManager::RenderManager(void) : currFPSSample(0),
										 resolution(800,600),
										 windowInFocus(true)
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
        openglSettings.depthBits = 16;
        openglSettings.stencilBits = 8;
        openglSettings.antialiasingLevel = 0;
        openglSettings.majorVersion = 3;
        openglSettings.minorVersion = 3;
		window = new RenderWindow(VideoMode(resolution.x, resolution.y),
								   "Epsilon Engine",
								   Style::Default,
								   openglSettings);

		// Set Framerate limit to 60fps
		window->setFramerateLimit(60);

		font = new Font();

//FIXME: This is the *worst*.  Fix ASAP after C++ ResourceManager is implemented.
#ifdef __APPLE__
        std::string fontPath = "/Users/scottporter/Development/Projects/C++/epsilonc/resources/sansation.ttf";
#else
        std::string fontPath = "resources/sansation.ttf";
#endif
		
        if (!font->loadFromFile(fontPath))
		{
			Log("Unable to find font: sansation.ttf");
		}
		else
		{
			// TODO: This needs to be moved into a UI Window with other debug info.
			fpsText = new Text(std::string("FPS: 0.0"), *font);
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
        
        fps = 0.0f;

		// Get an instance of the GizmoManager for rendering
		gizmoManager = &GizmoManager::GetInstance();
		
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

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);
        
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
        
		// Draw the Gizmos
		// Setup the Camera and Projection Matrix
		stateStack->State()->view = sceneManager->CurrentScene()->GetActiveCamera()->GetViewMatrix();
		stateStack->State()->projection = sceneManager->CurrentScene()->GetActiveCamera()->GetProjectionMatrix();
		gizmoManager->Draw(stateStack);
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

	bool RenderManager::WindowInFocus(void)
	{
		return windowInFocus;
	}

	bool RenderManager::PollEvent(sf::Event &event)
	{
		return window->pollEvent(event);
	}

	void RenderManager::ProcessEvent(sf::Event &event)
	{
		// Ignore input events if the window is not selected.
		switch (event.type)
		{
			// Check for window focus
			case sf::Event::GainedFocus:
				windowInFocus = true;
				break;
			case sf::Event::LostFocus:
				windowInFocus = false;
				break;
		}
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
