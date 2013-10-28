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
		scrolledwindow->SetRequisition( sf::Vector2f( 500.f, 100.f ) );

		// Add everything to our box.
		box->Pack( scrolledwindow, true, true );

		window = Window::Create();
		window->SetTitle("Console");
		window->Add(box);
	}

	void ConsoleWindow::OnUpdate(float seconds)
	{
		scrolledWindowBox->Update(seconds);
		scrolledwindow->Update(seconds);
		window->Update(seconds);
	}

	void ConsoleWindow::Log(string content)
	{
		scrolledWindowBox->Pack( Label::Create(content.c_str()) );
	}

	void ConsoleWindow::Log(string logName, string content)
	{
		content = logName + ": " + content;
		scrolledWindowBox->Pack( Label::Create(content.c_str()) );
	}
}