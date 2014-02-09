#include "script/python/UIModule.h"
#include "ui/UIWindow.h"
#include "ui/DebugWindow.h"
#include "ui/ConsoleWindow.h"

void initUI()
{
	using namespace epsilon;

	object uiModule(handle<>(borrowed(PyImport_AddModule("epsilon.ui"))));
	scope().attr("ui") = uiModule;
	scope uiScope = uiModule;

	// Expose the UIWindow Class
	class_<UIWindow, UIWindow::Ptr, boost::noncopyable>("UIWindow", no_init)
		.def("name", &UIWindow::GetName)
		.add_property("position", &UIWindow::GetPosition, &UIWindow::SetPosition)
		.add_property("visible", &UIWindow::IsVisible, &UIWindow::ShowWindow)
	;
}