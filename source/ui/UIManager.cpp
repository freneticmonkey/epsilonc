#include "ui/UIManager.h"
#include <algorithm>

namespace epsilon
{
	UIManager::UIManager()
	{

	}

	UIManager::~UIManager()
	{
		if (desktop) { delete desktop; }
		if ( sfgui ) { delete sfgui; }

	}

	void UIManager::Setup(void)
	{
		Log("Initialising UIManager");

		sfgui = new sfg::SFGUI();
		desktop = new sfg::Desktop();

	}

	//void UIManager::OnUpdate( sf::Time el )
	void UIManager::OnUpdate( float el )
	{
		//desktop->Update(el.asSeconds());
		desktop->Update(el);

		for (WindowListIterator i = windowList.begin(); i != windowList.end(); i++ )
		{
			//(*i)->OnUpdate(el.asSeconds());
			(*i)->OnUpdate(el);
		}
	}

	void UIManager::Draw(sf::RenderWindow * window)
	{
		// Setup the window to display the GUI

		window->resetGLStates();

		sfgui->Display( *window );
	}

	void UIManager::AddUIWindow(UIWindow::Ptr newWindow)
	{
		if ( newWindow )
		{
			desktop->Add(newWindow->GetWindow());
			windowList.push_back(newWindow);
		}
	}

	UIWindow::Ptr UIManager::GetWindowByName(const std::string & name)
	{
		UIWindow::Ptr window;

		WindowList::iterator foundChildIt = std::find_if(windowList.begin(), windowList.end(), [name](UIWindow::Ptr window){
			return window->GetName() == name;
		});

		if (foundChildIt != windowList.end())
		{
			window = (*foundChildIt);
		}

		return window;
	}

	void UIManager::ProcessEvent(sf::Event &event)
	{
		desktop->HandleEvent(event);
	}
}