#pragma once

#include "EpsilonCore.h"

#include "ui/UIManager.h"
#include "ui/RenderStatsWindow.h"

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
	class BGFXRenderManager
	{
		BGFXRenderManager(void);
	public:
		typedef std::vector<Renderer::Ptr>	Renderers;
		typedef std::vector<Light::Ptr>		Lights;
		typedef std::vector<Camera::Ptr>		Cameras;

		static BGFXRenderManager & GetInstance()
		{
			static BGFXRenderManager instance;
			return instance;
		}
		~BGFXRenderManager(void);

		void	Setup(void);
		void	OnUpdate(float el);
		void	Draw(float el);
		void	Destroy();

		// Exposed Window Methods
		bool	WindowOpen(void);
		bool	WindowInFocus(void);

		bool	PollEvent(sf::Event &event);

		bool	IsRunning() { return isRunning; }
		void	StopRunning(void);

		void    EnableUI(bool enabled = true);

		// Scene
		void	SetSceneManager(SceneManager * sm);

		// UI
		UIManager *	GetUIManager();

		// Utility
		float	GetFPS(float el);
		Vector2	GetResolution() { return resolution; }
		sf::RenderWindow *	GetWindow() { return window; }

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

		bool				isRunning;

		sf::RenderWindow *	window;
		Vector2				resolution;

		bool				windowInFocus;

		bool                enableUI;
		RenderStatsWindow::Ptr renderUI;
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

		// Current Event
		sf::Event event;

	};
}

