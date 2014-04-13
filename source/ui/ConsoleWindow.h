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
		public LogListener,
        public std::enable_shared_from_this<ConsoleWindow>
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<ConsoleWindow> Ptr;

		static Ptr Create();

		void Setup(void);
		void OnUpdate(float seconds);
        void Destroy();

		// Log Listener Functions
		void Log(std::string content);
		void Log(std::string logName, std::string content);

		explicit ConsoleWindow(const private_struct &);
	private:
		int maxLines;
		int currentLine;
		ConsoleLines consoleLines;
		ConsoleLines::iterator currLabel;
		ScrolledWindow::Ptr scrolledwindow;
		Box::Ptr scrolledWindowBox;
		Box::Ptr viewportBox;
		Alignment::Ptr scrolledWindowAlignment;
	};

}