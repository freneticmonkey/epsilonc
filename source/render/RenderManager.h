#pragma once

#include "EpsilonCore.h"

#include "ui/UIManager.h"
#include "scene/SceneManager.h"
#include "render/RenderState.h"
#include "render/gizmos/GizmoManager.h"

namespace epsilon
{
	class RenderManager
	{
		RenderManager(void);
	public:
		static RenderManager & GetInstance()
		{
			static RenderManager instance;
			return instance;
		}
		~RenderManager(void);

		void Setup(void);
		//void Draw(sf::Time el);
		void Draw(float el);

		// Exposed Window Methods
		bool WindowOpen(void);
		void CloseWindow(void);
		bool PollEvent(sf::Event &event);

		// Scene
		void SetSceneManager(SceneManager * sm);

		// UI
		void SetUIManager(UIManager *uim);

		// Utility
		float GetFPS(float el);

		Vector2 GetResolution() { return resolution;  }

		sf::RenderWindow * GetWindow() { return window;  }

	private:
		sf::RenderWindow *	window;
		sf::Text *			fpsText;
		sf::Font *			font;
		Vector2				resolution;

		RenderStateStack::Ptr stateStack;

		float				fps;
		SceneManager *		sceneManager;
		UIManager *			uiManager;

		GizmoManager *		gizmoManager;

		static const int	NUM_FPS_SAMPLES = 64;
		float				fpsSamples[NUM_FPS_SAMPLES];
		int					currFPSSample;
	};
}

