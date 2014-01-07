#include "ui/DebugWindow.h"

namespace epsilon
{
	DebugWindow::Ptr DebugWindow::Create(void) 
	{
		return std::make_shared<DebugWindow>( private_struct() );
	}

	DebugWindow::Ptr DebugWindow::Create(std::string title) 
	{
		return std::make_shared<DebugWindow>(private_struct(), title);
	}

	DebugWindow::DebugWindow(const private_struct &) 
	{
		windowTitle = std::string("Debug Window");
	}
	
	DebugWindow::DebugWindow(const private_struct &, std::string title)
	{
		windowTitle = title;
	}

	void DebugWindow::Setup(void)
	{
		label = Label::Create("Hello World!");
		Button::Ptr button( Button::Create("Click Me") );
		button->GetSignal( Widget::OnLeftClick ).Connect( &DebugWindow::OnButtonClick, this );

		Box::Ptr box(Box::Create( Box::VERTICAL, 5.0f ) );
		box->Pack( label );
		box->Pack( button, false );

		window = Window::Create();
		window->SetTitle(windowTitle);
		window->Add(box);
	}

	void DebugWindow::OnUpdate(float seconds)
	{

	}

	void DebugWindow::OnButtonClick()
	{
		label->SetText("Button was clicked");
	}
	
}