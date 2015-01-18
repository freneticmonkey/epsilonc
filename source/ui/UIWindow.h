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
		bool titlebar = true;
		bool border = false;
		bool resize = true;
		bool move = true;
		bool scrollbar = true;
		float fill_alpha = 0.65f;
		Vector2	position = Vector2(0, 0);
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
			const ImGuiWindowFlags layout_flags = (!options.titlebar ? ImGuiWindowFlags_NoTitleBar : 0) | 
												  (options.border ? ImGuiWindowFlags_ShowBorders : 0 ) |
												  (!options.resize ? ImGuiWindowFlags_NoResize : 0) | 
												  (!options.move ? ImGuiWindowFlags_NoMove : 0) | 
												  (!options.scrollbar ? ImGuiWindowFlags_NoScrollbar : 0);

			if (ImGui::Begin(title.c_str(), &options.is_open, ImVec2(options.position.x, options.position.y), options.fill_alpha, layout_flags))
			{
				// Call Overridden GUI Function
				OnGUI(el);
			}
			ImGui::End();
		};

		std::string GetTitle() { return title; }

		void OpenWindow(bool show = true) { options.is_open = show; }
		bool IsOpen() { return options.is_open; }

		void ShowTitlebar(bool show = true) { options.titlebar = show; }
		void ShowBorder(bool show = true) { options.border = show; }
		void ResizeEnabled(bool resize = true) { options.resize = resize; }
		void MoveEnabled(bool moveEnabled = true) { options.move = moveEnabled; }
		void ScrollEnabled(bool scrollEnabled = true) { options.scrollbar = scrollEnabled; }

		void SetPosition(Vector2 newPosition)
		{
			options.position = newPosition;
		}

		Vector2 GetPosition()
		{
			return options.position;
		}

	protected:
		virtual void OnGUI(float el) = 0;

		std::string		title;
		WindowOptions	options;
	};

}