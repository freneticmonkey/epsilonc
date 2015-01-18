#include "ui/UIManager.h"
#include "render/RenderUtilities.h"
#include <algorithm>

// Include IMGUI and SFML IMGUI Wrapper
#include "external/imgui/imgui.h"
#include "external/imgui-backends/SFML/imgui-SFML.h"

namespace epsilon
{
	UIManager::UIManager()
	{

	}

	UIManager::~UIManager()
	{
		
	}

	void UIManager::Setup(sf::RenderWindow * window)
	{
		Log("Initialising UIManager");

		ImGui::SFML::SetWindow(*window);
		ImGui::SFML::InitImGui();
	}

	void UIManager::Destroy()
	{
		std::for_each(windowList.begin(), windowList.end(), [](UIWindow::Ptr window){
			window->Destroy();
		});
	}

	void UIManager::OnUpdate(float el)
	{
		// FIXME: Avoiding Manager initialisation ordering issues
		if (!scriptManager)
		{
			scriptManager = &ScriptManager::GetInstance();
		}

		ImGui::SFML::UpdateImGui();

		// Process any windows
		for (WindowListIterator i = windowList.begin(); i != windowList.end(); i++)
		{
			(*i)->OnUpdate(el);
		}

		// Process GUI calls from scripts
		if (scriptManager)
		{
			scriptManager->BehaviourOnGUI();
		}
	}

	void UIManager::Draw()
	{
		// Render IMGUI
		ImGui::Render();
	}

	void UIManager::AddUIOverlay(UIOverlay::Ptr newOverlay)
	{
		if (newOverlay)
		{
			overlayList.push_back(newOverlay);
		}
	}

	UIOverlay::Ptr UIManager::GetOverlayByName(const std::string & name)
	{
		UIOverlay::Ptr overlay;

		OverlayList::iterator foundChildIt = std::find_if(overlayList.begin(), overlayList.end(), [name](UIOverlay::Ptr overlay){
			return overlay->GetName() == name;
		});

		if (foundChildIt != overlayList.end())
		{
			overlay = (*foundChildIt);
		}

		return overlay;
	}

	void UIManager::AddUIWindow(UIWindow::Ptr newWindow)
	{
		if (newWindow)
		{
			windowList.push_back(newWindow);
		}
	}

	UIWindow::Ptr UIManager::GetWindowByName(const std::string & name)
	{
		UIWindow::Ptr window;

		WindowList::iterator foundChildIt = std::find_if(windowList.begin(), windowList.end(), [name](UIWindow::Ptr window){
			return window->GetTitle() == name;
		});

		if (foundChildIt != windowList.end())
		{
			window = (*foundChildIt);
		}

		return window;
	}

	void UIManager::ProcessEvent(sf::Event &event)
	{
		ImGui::SFML::ProcessEvent(event);
	}
}