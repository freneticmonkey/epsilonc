#pragma once

#include "EpsilonCore.h"

#include "ui/UIManager.h"
#include "scene/Scenemanager.h"

namespace epsilon
{
	class RenderManager
	{

	public:
		RenderManager(void);
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

	private:
		sf::RenderWindow * window;
		sf::Text * fpsText;
		sf::Font * font;
		float fps;
		SceneManager * sceneManager;
		UIManager * uiManager;
	};
}

