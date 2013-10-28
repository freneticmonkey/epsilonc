#pragma once
#include "EpsilonCore.h"

#include <SFGUI/SFGUI.hpp>
#include <list>
#include <memory>

#include "logging/Logging.h"
#include "ui/uiwindow.h"

namespace epsilon
{
	using namespace sfg;

	typedef list<Label::Ptr> ConsoleLines;

	class ConsoleWindow :
		public UIWindow, 
		public LogListener
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<ConsoleWindow> Ptr;

		static Ptr Create();

		void Setup(void);
		void OnUpdate(float seconds);

		// Log Listener Functions
		void Log(string content);
		void Log(string logName, string content);

		explicit ConsoleWindow(const private_struct &);
	private:
		
		ConsoleLines consoleLines;
		ScrolledWindow::Ptr scrolledwindow;
		Box::Ptr scrolledWindowBox;
		Box::Ptr viewportBox;
	};

}