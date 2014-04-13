#pragma once

#include "EpsilonCore.h"
#include "ui/uiwindow.h"

namespace epsilon
{
	using namespace sfg;

	class DebugWindow :
		public UIWindow
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<DebugWindow> Ptr;
		
		static Ptr Create();
		static Ptr Create(std::string title);

		void Setup(void);
		void OnUpdate(float seconds);
        void Destroy();

		void OnButtonClick();
		
		explicit DebugWindow(const private_struct &);
		DebugWindow(const private_struct &, std::string title);

	private:
		Label::Ptr label;
		std::string	   windowTitle;
	};

}
