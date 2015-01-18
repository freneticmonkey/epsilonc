#pragma once

#include <SFML/Graphics.hpp>

#include "EpsilonCore.h"
#include "logging/Logging.h"
#include "ui/UIWindow.h"
#include "ui/UIOverlay.h"

namespace epsilon
{
	typedef std::list<UIWindow::Ptr> WindowList;
	typedef WindowList::iterator WindowListIterator;

	typedef std::list<UIOverlay::Ptr> OverlayList;

	class UIManager
	{
	private:
		UIManager();
		UIManager(UIManager const&);
		void operator=(UIManager const&);

	public:
		static UIManager & GetInstance()
		{
			static UIManager instance;
			return instance;
		};

		~UIManager();

		void Setup(sf::RenderWindow * window);
		void OnUpdate(float el);
		void Draw();
        void Destroy();

		void AddUIOverlay(UIOverlay::Ptr newOverlay);
		UIOverlay::Ptr GetOverlayByName(const std::string & name);

		void AddUIWindow(UIWindow::Ptr newWindow);
		UIWindow::Ptr GetWindowByName(const std::string & name);
		void ProcessEvent(sf::Event &event);

	private:
		WindowList windowList;
		OverlayList overlayList;
	};
}
