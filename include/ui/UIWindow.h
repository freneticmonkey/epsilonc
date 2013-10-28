#pragma once

#include "EpsilonCore.h"

#include <SFGUI/SFGUI.hpp>

namespace epsilon
{
	class UIWindow
	{
	public:
		typedef std::shared_ptr<UIWindow> Ptr;

		virtual ~UIWindow() {};

		virtual void Setup(void) = 0;

		virtual void OnUpdate(float seconds) = 0;

		sfg::Window::Ptr GetWindow() { return window; }

		void SetTitle(std::string title)
		{
			if (window)
			{
				window->SetTitle(title);
			}
		}

	protected:
		sfg::Window::Ptr window;
	};

}