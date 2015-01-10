#pragma once

#include "EpsilonCore.h"

#include "ui/UIManager.h"
#include "scene/SceneManager.h"
#include "render/Renderer.h"
#include "render/gizmos/GizmoManager.h"
#include "render/material/ShaderManager.h"
#include "render/mesh/MeshManager.h"
#include "render/texture/TextureManager.h"
#include "render/mesh/VertexData.h"
#include "render/Light.h"
#include "render/Camera.h"

namespace epsilon
{
	class RenderManager
	{
		RenderManager(void);
	public:
		typedef std::vector<Renderer::Ptr>	Renderers;
		typedef std::vector<Light::Ptr>		Lights;
		typedef std::vector<Camera::Ptr>		Cameras;

		static RenderManager & GetInstance()
		{
			static RenderManager instance;
			return instance;
		}
		~RenderManager(void);
		
		void	Setup(void);
		void	Draw(float el);        
        void	Destroy();

		// Exposed Window Methods
		bool	WindowOpen(void);
		void	CloseWindow(void);
		bool	WindowInFocus(void);

		bool	PollEvent(sf::Event &event);

		void	ProcessEvent(sf::Event &event);

		// Scene
		void	SetSceneManager(SceneManager * sm);

		// UI
		void	SetUIManager(UIManager *uim);

		// Utility
		float	GetFPS(float el);
		Vector2	GetResolution() { return resolution;  }
		sf::RenderWindow *	GetWindow() { return window;  }
        
		// Create Managed Items
		Renderer::Ptr		CreateRenderer();
		bool				DestroyRenderer(Renderer::Ptr renderer);
		
		Light::Ptr			CreateLight(std::string name);
		bool				DestroyLight(Light::Ptr newLight);
		bool				DestroyLight(std::string name);

		Camera::Ptr			CreateCamera(std::string name);
        
	private:
		void	ProcessCameras();
		void	SetupLights();
		void	TeardownLights();

		sf::RenderWindow *	window;
		sf::Text *			fpsText;
		sf::Font *			font;
		Vector2				resolution;

		bool				windowInFocus;

		float				fps;
		SceneManager *		sceneManager;
		UIManager *			uiManager;
        ShaderManager *     shaderManager;
		TextureManager *	textureManager;
		MeshManager *		meshManager;

		GizmoManager *		gizmoManager;

		static const int	NUM_FPS_SAMPLES = 64;
		float				fpsSamples[NUM_FPS_SAMPLES];
		int					currFPSSample;
        
		Renderers			renderItems;

        Renderers           renderers;

		ShaderUniform::Ptr	numLights;
		Lights				lights;
		Cameras				cameras;
	};
}

