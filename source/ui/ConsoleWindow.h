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

	typedef std::list<Label::Ptr> ConsoleLines;

	class ConsoleWindow :
		public UIWindow, 
		public LogListener
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<ConsoleWindow> Ptr;

		static Ptr Create();

		void Setup(void);
		void OnUpdate(float seconds);

		// Log Listener Functions
		void Log(std::string content);
		void Log(std::string logName, std::string content);

		explicit ConsoleWindow(const private_struct &);
	private:
		
		ConsoleLines consoleLines;
		ScrolledWindow::Ptr scrolledwindow;
		Box::Ptr scrolledWindowBox;
		Box::Ptr viewportBox;
		Alignment::Ptr scrolledWindowAlignment;
	};

}