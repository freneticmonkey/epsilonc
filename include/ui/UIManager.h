#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "ui/UIWindow.h"

namespace epsilon
{
	using namespace sfg;
	using namespace std;

	typedef list<UIWindow::Ptr> WindowList;
	typedef WindowList::iterator WindowListIterator;

	class UIManager
	{
	public:
		UIManager();
		~UIManager();

		void Setup(void);
		//void OnUpdate(sf::Time el);
		void OnUpdate(float el);
		void Draw(sf::RenderWindow * window);

		void AddUIWindow(UIWindow::Ptr newWindow);

		void ProcessEvent(sf::Event &event);

	private:
		SFGUI * sfgui;
		Desktop * desktop;
		WindowList windowList;
	};
}