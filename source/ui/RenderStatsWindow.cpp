#include "ui/RenderStatsWindow.h"
#include <boost/format.hpp>

namespace epsilon
{
	RenderStatsWindow::Ptr RenderStatsWindow::Create(void)
	{
		return std::make_shared<RenderStatsWindow>(private_struct());
	}

	RenderStatsWindow::RenderStatsWindow(const private_struct &) : UIWindow("Render Stats")
	{
		ShowTitlebar(false);
		MoveEnabled(false);
		ResizeEnabled(false);
	}

	void RenderStatsWindow::Setup(void)
	{
		
	}

	void RenderStatsWindow::Destroy(void)
	{

	}

	void RenderStatsWindow::OnGUI(float seconds)
	{
		ImGui::Text(boost::str(format("FPS: %f") % fps).c_str());
	}

}