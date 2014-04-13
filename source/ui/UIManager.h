#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "ui/UIWindow.h"
#include "ui/UIOverlay.h"
#include "ui/Graph.h"

namespace epsilon
{
	typedef std::list<UIWindow::Ptr> WindowList;
	typedef WindowList::iterator WindowListIterator;

	typedef std::list<Graph::Ptr> GraphList;
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

		void Setup(void);
		//void OnUpdate(sf::Time el);
		void OnUpdate(float el);
		void Draw(sf::RenderWindow * window);
        void Destroy();

		void AddUIOverlay(UIOverlay::Ptr newOverlay);
		UIOverlay::Ptr GetOverlayByName(const std::string & name);

		void AddUIWindow(UIWindow::Ptr newWindow);
		UIWindow::Ptr GetWindowByName(const std::string & name);
		void ProcessEvent(sf::Event &event);

	private:
		sfg::SFGUI * sfgui;
		sfg::Desktop * desktop;
		WindowList windowList;
		OverlayList overlayList;
	};
}
