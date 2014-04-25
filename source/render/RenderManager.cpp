#include "render/RenderManager.h"
#include "render/gizmos/GizmoCube.h"
#include <boost/format.hpp>

using namespace boost;

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
        openglSettings.minorVersion = 2;
		window = new RenderWindow(VideoMode(resolution.x, resolution.y),
								   "Epsilon Engine",
								   Style::Default,
								   openglSettings);
        
        CheckOpenGLError("After creating window");

		// Set Framerate limit to 60fps
		window->setFramerateLimit(60);

		font = new Font();

//FIXME: This is the *worst*.  UI only on Windows build for now.
#ifndef __APPLE__
        std::string fontPath = "resources/sansation.ttf";
		
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
#endif
//		window->setVerticalSyncEnabled(true);

		// Initialising OpenGL
        
        GLenum GlewInitResult;
        
        CheckOpenGLError("Before init GLEW");
        
        glewExperimental = GL_TRUE;
        
        //FIXME: Potential crash issue here.
        GlewInitResult = glewInit();

        CheckOpenGLError("After init GLEW");
        
		if (GLEW_OK != GlewInitResult) 
		{
			Log("ERROR: Error initialising GLEW");
			Log((const char *)glewGetErrorString(GlewInitResult));
		}
        
        Log("INFO: OpenGL Vendor: ");
		Log((const char *)glGetString(GL_VENDOR));
		Log("INFO: OpenGL Version: ");
		Log((const char *)glGetString(GL_VERSION));
        Log("INFO: OpenGL Renderer: ");
		Log((const char *)glGetString(GL_RENDERER));
        Log("INFO: OpenGL Shading Version: ");
        Log((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        CheckOpenGLError("Getting OpenGl Versions");
        
        fps = 0.0f;

		// Get an instance of the GizmoManager for rendering
		gizmoManager = &GizmoManager::GetInstance();
		
        // Get an instance of the ShaderManager for rendering
        shaderManager = &ShaderManager::GetInstance();

		// Get an instance of the TextureManager
		textureManager = &TextureManager::GetInstance();

		// Get an instance of the MeshManager
		meshManager = &MeshManager::GetInstance();
	}

	void RenderManager::Draw(float el)
	{
        // Make this window active
		window->setActive(true);

		// Clear the window
		window->clear(sf::Color(50,50,50,255));

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		// Obtain the list of items to be drawn
		renderItems = sceneManager->GetRenderList();

		// Push any texture changes to the GPU
		// On the first frame this will take some time.
		textureManager->ProcessTextures();

		// Push any mesh changes to the GPU
		// On the first frame this will take some time.
		meshManager->ProcessMeshs();

        //Sync Camera Data with the Uniform Buffer Objects
		ProcessCameras();

		// Sync Light Data with the Uniform Buffer Objects and
		// render shadow depth maps
		SetupLights();
        
		// Perform the Uniform Buffer Copy
        shaderManager->ProcessUniformBuffers();
        
		// Do OpenGL drawing here.
		std::for_each(renderItems.begin(), renderItems.end(), [](Renderer::Ptr renderer){
			renderer->Draw();
		});
        
		// Perform post render light cleanup
		TeardownLights();

        std::string output = boost::str(format("Epsilon - FPS: %f") % GetFPS(el) );
        window->setTitle(output);
        
        if ( fpsText )
		{
            window->pushGLStates();
            window->resetGLStates();
            window->draw(*fpsText);
            window->popGLStates();
		}
        
        // Draw the Gizmos
		gizmoManager->Draw();
		
        // Draw the GUI
		if ( uiManager )
		{
		//	uiManager->Draw(window);
		}
        
		// Display the frame
		window->display();
	}
    
    void RenderManager::Destroy()
    {
        // Clear errors before drawing gizmos to be sure we get real errors
        glGetError();
        
        // destroy the gizmo manager first
        gizmoManager->Destroy();
        
        // Pass on the destroy message to the Renderer objects
        std::for_each(renderers.begin(), renderers.end(), [](Renderer::Ptr renderer){
            renderer->Destroy();
        });
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
        
#ifndef __APPLE__
        // Disable setting the UIManager on OSX as the UI isn't working.
		if ( uim )
		{
			uiManager = uim;
		}
#endif
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

	Renderer::Ptr RenderManager::CreateRenderer()
	{
		Renderer::Ptr newRenderer = Renderer::Create();
		renderers.push_back(newRenderer);
		return newRenderer;
	}

	Light::Ptr RenderManager::CreateLight(std::string name)
	{
		Light::Ptr newLight = Light::Create(lights.size(), name);
		lights.push_back(newLight);
		return newLight;
	}

	Camera::Ptr RenderManager::CreateCamera(std::string name)
	{
		Camera::Ptr newCamera = Camera::Create(name);
		cameras.push_back(newCamera);
		return newCamera;
	}

	void RenderManager::ProcessCameras()
	{
		std::for_each(cameras.begin(), cameras.end(), [](Camera::Ptr camera){
			if (camera->IsActive())
			{
				camera->Update();
			}
		});
	}

	void RenderManager::SetupLights()
	{
		// For each light, push info into the uniform buffer.
		if (numLights)
		{
			numLights->SetInt(lights.size());

			std::for_each(lights.begin(), lights.end(), [&](Light::Ptr light){
				light->PreRender(renderItems);
			});
		}	
		else
		{
			// Get a pointer to the numLights property in the Lights Uniform Buffer
			UniformBuffer::Ptr lights = shaderManager->GetUniformBuffer("Lights");
			if (lights)
			{
				numLights = lights->GetUniform("numLights");
			}
		}
	}

	void RenderManager::TeardownLights()
	{
		std::for_each(lights.begin(), lights.end(), [](Light::Ptr light){
			light->PostRender();
		});
	}


}
