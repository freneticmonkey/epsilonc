#include "ui/ConsoleWindow.h"

namespace epsilon
{
	ConsoleWindow::Ptr ConsoleWindow::Create()
	{
		Ptr newConsoleWindow = make_shared<ConsoleWindow>(private_struct());
		Logger::addListener(newConsoleWindow);
		return newConsoleWindow;
	}

	ConsoleWindow::ConsoleWindow(const private_struct &) : LogListener()
	{
	}
	
	void ConsoleWindow::Setup(void)
	{
		
		// Create a box with 10 pixel spacing.
		Box::Ptr box = Box::Create( Box::VERTICAL, 10.f );

		scrolledWindowBox = Box::Create( Box::VERTICAL );

		// Create the ScrolledWindow.
		scrolledwindow = ScrolledWindow::Create();

		// Set the ScrolledWindow to always show the horizontal scrollbar
		// and only show the vertical scrollbar when needed.
		scrolledwindow->SetScrollbarPolicy( ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_ALWAYS );

		// Add the ScrolledWindow box to the ScrolledWindow
		// and create a new viewport automatically.
		scrolledwindow->AddWithViewport( scrolledWindowBox );

		// Always remember to set the minimum size of a ScrolledWindow.
		scrolledwindow->SetRequisition( sf::Vector2f( 600.f, 200.f ) );

		// Add alignment to the window
		scrolledWindowAlignment = Alignment::Create();
		scrolledWindowBox->Pack( scrolledWindowAlignment, true, true );
		scrolledWindowAlignment->SetAlignment( sf::Vector2f(0.0f, 0.0f) );

		// Add everything to our box.
		box->Pack( scrolledwindow, true, true );

		// Add the Console to the UI
		window = Window::Create();
		window->SetTitle("Console");
		window->Add(box);

		// Fill the console with all previous log messages to ensure that it's up-to-date
		LogList initLog = Logger::FlushInitLog();

		for ( LogList::iterator log = initLog.begin(); log != initLog.end(); log++)
		{
			this->Log( (*log) );
		}
	}

	void ConsoleWindow::OnUpdate(float seconds)
	{
		scrolledWindowBox->Update(seconds);
		scrolledwindow->Update(seconds);
		window->Update(seconds);
	}

	void ConsoleWindow::Log(string content)
	{
		Label::Ptr newLog = Label::Create(content.c_str());
		scrolledWindowBox->Pack( newLog );
		newLog->SetAlignment( sf::Vector2f(0.0f, 0.0f) );
	}

	void ConsoleWindow::Log(string logName, string content)
	{
		content = logName + ": " + content;
		Log(content);
	}
}