#include "ui/ConsoleWindow.h"

namespace epsilon
{
	ConsoleWindow::Ptr ConsoleWindow::Create()
	{
		Ptr newConsoleWindow = std::make_shared<ConsoleWindow>(private_struct());
		Logger::getInstance().addListener(newConsoleWindow);
		return newConsoleWindow;
	}

	ConsoleWindow::ConsoleWindow(const private_struct &) : UIWindow("console"),
														   LogListener(), 
														   maxLines(25), 
														   currentLine(0)
	{
	}
	
	void ConsoleWindow::Setup(void)
	{
		
		// Create a box with 10 pixel spacing.
		Box::Ptr box = Box::Create( Box::Orientation::VERTICAL, 10.f );

		scrolledWindowBox = Box::Create(Box::Orientation::VERTICAL);

		// Create the ScrolledWindow.
		scrolledwindow = ScrolledWindow::Create();

		// Set the ScrolledWindow to always show the horizontal scrollbar
		// and only show the vertical scrollbar when needed.
		scrolledwindow->SetScrollbarPolicy( ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_ALWAYS | ScrolledWindow::VERTICAL_AUTOMATIC  );

		// Add the ScrolledWindow box to the ScrolledWindow
		// and create a new viewport automatically.
		scrolledwindow->AddWithViewport( scrolledWindowBox );

		// Always remember to set the minimum size of a ScrolledWindow.
		scrolledwindow->SetRequisition( sf::Vector2f( 600.f, 200.f ) );

		// Add alignment to the window
		scrolledWindowAlignment = Alignment::Create();
		scrolledWindowBox->Pack( scrolledWindowAlignment, true, true );
		scrolledWindowAlignment->SetAlignment( sf::Vector2f(0.0f, 0.0f) );

		// Get a list of all previous log messages
		LogList initLog = Logger::getInstance().FlushInitLog();
		
		// resize init log to console maxLines
		while (initLog.size() > maxLines)
		{
			initLog.pop_front();
		}
		LogList::iterator initLogMsg = initLog.begin();

		std::string logMessage;

		// Generate maxLines Labels
		for ( int i = 0; i < maxLines; i++ )
		{
			// Get a init msg.
			if (initLogMsg != initLog.end())
			{
				logMessage = *initLogMsg;
				initLogMsg++;
			}
			else
			{
				logMessage = "";
			}

			Label::Ptr newLog = Label::Create(logMessage);
			consoleLines.push_back(newLog);
		}
		currLabel = consoleLines.begin();

		// Add everything to our box.
		box->Pack( scrolledwindow, true, true );

		// Add the Console to the UI
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
    
    void ConsoleWindow::Destroy()
    {
        // Stop listening to Logger
        Logger::getInstance().	removeListener(shared_from_this());
    }

	void ConsoleWindow::Log(std::string content)
	{
		if (false)
		{
			Label::Ptr logLbl = Label::Create(content.c_str());
			scrolledWindowBox->Pack(logLbl);// , false, true );
			logLbl->SetAlignment(sf::Vector2f(0.0f, 0.0f));
		}
		else
		{
			if ( currentLine < maxLines )
			{
				// Push a new Label into the console box
				Label::Ptr logLbl = (*currLabel);
				logLbl->SetText(content.c_str());
				scrolledWindowBox->Pack(logLbl);// , false, true );
				logLbl->SetAlignment( sf::Vector2f(0.0f, 0.0f) );

				currentLine++;
			}
			else
			{
				// Otherwise all of the labels have been added to the console,
				// just update the text and reorder it
				(*currLabel)->SetText(content.c_str());
				scrolledWindowBox->ReorderChild((*currLabel), maxLines - 1);
			}

			currLabel++;
			// Iterate to the next console line ensuring wrapping.
			if (currLabel == consoleLines.end())
			{
				currLabel = consoleLines.begin();
			} 
		}
	}

	void ConsoleWindow::Log(std::string logName, std::string content)
	{
		// Ignore the python std out as it is most likely for debugging
		if (logName != "python")
		{
			content = logName + ": " + content;
			Log(content);
		}
	}
}