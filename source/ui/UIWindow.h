#pragma once

#include "EpsilonCore.h"
#include "math/Vector.h"

#include "external/imgui/imgui.h"

namespace epsilon
{
	class WindowOptions
	{
	public:
		bool is_open = true;
		bool no_titlebar = false;
		bool no_border = true;
		bool no_resize = false;
		bool no_move = false;
		bool no_scrollbar = false;
		float fill_alpha = 0.65f;
		WindowOptions(){}
	};

	class UIWindow
	{
	public:

		UIWindow(std::string windowTitle,
				 WindowOptions windowOptions = WindowOptions()) : title(windowTitle),
																  options(windowOptions){}

		typedef std::shared_ptr<UIWindow> Ptr;

		virtual ~UIWindow() {};

		virtual void Setup(void) = 0;
		virtual void Destroy(void) = 0;

		void OnUpdate(float el)
		{
			const ImGuiWindowFlags layout_flags = (options.no_titlebar ? ImGuiWindowFlags_NoTitleBar : 0) | 
												  (options.no_border ? 0 : ImGuiWindowFlags_ShowBorders) | 
												  (options.no_resize ? ImGuiWindowFlags_NoResize : 0) | 
												  (options.no_move ? ImGuiWindowFlags_NoMove : 0) | 
												  (options.no_scrollbar ? ImGuiWindowFlags_NoScrollbar : 0);

			if (ImGui::Begin(title.c_str(), &options.is_open, ImVec2(position.x, position.y), options.fill_alpha, layout_flags))
			{
				// Call Overridden GUI Function
				OnGUI(el);
			}
			ImGui::End();
		};

		std::string GetTitle() { return title; }

		void OpenWindow(bool show = true) { options.is_open = show; }
		bool IsOpen() { return options.is_open; }

		void ShowTitlebar(bool show = true) { options.no_titlebar = !show; }
		void ShowBorder(bool show = true) { options.no_border = !show; }
		void ResizeEnabled(bool resize = true) { options.no_resize = !resize; }
		void MoveEnabled(bool moveEnabled = true) { options.no_move = !moveEnabled; }
		void ScrollEnabled(bool scrollEnabled = true) { options.no_scrollbar = !scrollEnabled; }

		void SetPosition(Vector2 newPosition)
		{
			position = newPosition;
		}

		Vector2 GetPosition()
		{
			return position;
		}

	protected:
		virtual void OnGUI(float el) = 0;

		std::string		title;
		WindowOptions	options;
		Vector2			position;
	};

}