#include <boost/format.hpp>

// bgfx
#include <external/bgfx/include/bgfx.h>

#include "render/RenderManager.h"
#include "render/gizmos/GizmoCube.h"
#include "ui/UIManager.h"


using namespace boost;

namespace epsilon
{
	RenderManager::RenderManager(void) : currFPSSample(0),
										 resolution(800,600),
										 windowInFocus(true),
										 isRunning(true),
                                         enableUI(true)
	{
		for ( int i = 0; i < NUM_FPS_SAMPLES; i++)
		{
			fpsSamples[i] = 0.0f;
		}
	}

	RenderManager::~RenderManager(void)
	{
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
											"Epsilon Engine" ,
											Style::Default,
											openglSettings);

        CheckOpenGLError("After creating window");

		// Now that we have the window initialise the UIManager
		uiManager = &UIManager::GetInstance();
		uiManager->Setup(window);

		// Set Framerate limit to 60fps
		window->setFramerateLimit(60);

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
     
		renderUI = RenderStatsWindow::Create();
		uiManager->AddUIWindow(renderUI);
        fps = 0.0f;

		// Create an instance of the GizmoManager for rendering Gizmos
		gizmoManager = &GizmoManager::GetInstance();
		gizmoManager->Setup();
		
        // Get an instance of the ShaderManager for rendering
        shaderManager = &ShaderManager::GetInstance();

		// Get an instance of the TextureManager
		textureManager = &TextureManager::GetInstance();

		// Get an instance of the MeshManager
		meshManager = &MeshManager::GetInstance();
	}

	void RenderManager::OnUpdate(float el)
	{
		gizmoManager->OnUpdate(el);
        if ( enableUI )
        {
            uiManager->OnUpdate(el);
        }
	}

	void RenderManager::Draw(float el)
	{
        // Make this window active
		window->setActive(true);
		
        // Clear errors before drawing gizmos to be sure we get real errors
        glGetError();
        
		// Clear the window
		//window->clear(sf::Color(50,50,50,255));

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
		meshManager->ProcessMeshes();
		
		// Sync Light Data with the Uniform Buffer Objects and
		// render shadow depth maps
		SetupLights();

		//Sync Camera Data with the Uniform Buffer Objects
		ProcessCameras();
        
		// Perform the Uniform Buffer Copy
        shaderManager->ProcessUniformBuffers();
        
		// Do OpenGL drawing here.
		std::for_each(renderItems.begin(), renderItems.end(), [](Renderer::Ptr renderer){
			renderer->Draw();
		});
        
		// Perform post render light cleanup
		TeardownLights();

		// Pass the FPS Stats through to the render UI
		renderUI->SetFPS(GetFPS(el));
		        
        // Draw the Gizmos
		gizmoManager->Draw();
        
        if ( enableUI )
        {
            // Draw the UI
            uiManager->Draw();
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


		uiManager->Destroy();
        
        // Pass on the destroy message to the Renderer objects
        std::for_each(renderers.begin(), renderers.end(), [](Renderer::Ptr renderer){
            renderer->OnDestroy();
        });

		if (window)
		{
			window->close();
		}
    }

	bool RenderManager::WindowOpen(void)
	{
		return ( window ) ? window->isOpen() : false;
	}

	void RenderManager::StopRunning(void)
	{
		isRunning = false;
	}

	bool RenderManager::WindowInFocus(void)
	{
		return windowInFocus;
	}
    
    void RenderManager::EnableUI(bool enabled)
    {
        enableUI = enabled;
    }
	
	bool RenderManager::PollEvent(sf::Event &event)
	{
		// Grab an event off the queue
		bool hasEvent = window->pollEvent(event);

		if (hasEvent)
		{
			// Process the event internally via RenderManager and UIManager
			// before returning
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

			// If the window is in focus
			if (windowInFocus)
			{
                if ( enableUI )
                {
                    // Process the UI events
                    uiManager->ProcessEvent(event);
                }
			}

			// If a close event has been detected
			if (event.type == sf::Event::Closed)
			{
				isRunning = false;

				// Immediately Break the event pump? - This could result in unexpected behaviour
				hasEvent = false;
			}
		}
		return hasEvent;
	}

	void RenderManager::SetSceneManager(SceneManager * sm)
	{
		sceneManager = sm;
	}

	UIManager * RenderManager::GetUIManager()
	{
		return uiManager;
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

	bool RenderManager::DestroyRenderer(Renderer::Ptr renderer)
	{
		bool success = false;

		//Renderer::Ptr newRenderer = Renderer::Create();
		Renderers::iterator it = std::find_if(renderers.begin(), renderers.end(), [&](Renderer::Ptr rend){
			return rend == renderer;
		});

		if (it != renderers.end())
		{
			(*it)->OnDestroy();
			renderers.erase(it);
			success = true;
		}

		return success;
	}

	Light::Ptr RenderManager::CreateLight(std::string name)
	{
		Light::Ptr newLight = Light::Create(lights.size(), name);
		lights.push_back(newLight);
		return newLight;
	}

	bool RenderManager::DestroyLight(Light::Ptr light)
	{
		bool success = false;

		LightList::iterator it = std::find_if(lights.begin(), lights.end(), [&](Light::Ptr sLight){
			return light == sLight;
		});

		if (it != lights.end())
		{
			lights.erase(it);
			success = true;
		}

		return success;
	}

	bool RenderManager::DestroyLight(std::string name)
	{
		bool success = false;

		LightList::iterator it = std::find_if(lights.begin(), lights.end(), [&](Light::Ptr light){
			return light->GetName() == name;
		});

		if (it != lights.end())
		{
			lights.erase(it);
			success = true;
		}

		return success;
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
