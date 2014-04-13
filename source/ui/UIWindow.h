#pragma once

#include "EpsilonCore.h"
#include "math/Vector.h"

#include <SFGUI/SFGUI.hpp>

namespace epsilon
{
	class UIWindow
	{
	public:

		UIWindow(std::string windowName) : name(windowName) {}

		typedef std::shared_ptr<UIWindow> Ptr;

		virtual ~UIWindow() {};

		virtual void Setup(void) = 0;

		virtual void OnUpdate(float seconds) = 0;
        
        virtual void Destroy() = 0;

		sfg::Window::Ptr GetWindow() { return window; }
		std::string GetName() { return name; }

		// Window Operations
		void SetTitle(std::string title)
		{
			if (window)
			{
				window->SetTitle(title);
			}
		}

		void ShowWindow(bool show)
		{
			if (window)
			{
				window->Show(show);
			}
		}

		bool IsVisible()
		{
			bool visible = false;
			if (window)
			{
				visible = window->IsGloballyVisible();
			}
			return visible;
		}

		void SetPosition(Vector2 position)
		{
			if (window)
			{
				window->SetPosition(sf::Vector2f(position.x, position.y));
			}
		}

		Vector2 GetPosition()
		{
			Vector2 pos;
			if (window)
			{
				sf::Vector2f wPos = window->GetAbsolutePosition();
				pos.x = wPos.x;
				pos.y = wPos.y;
			}
			return pos;
		}

	protected:
		sfg::Window::Ptr window;
		std::string name;
	};

}