#pragma once

#include "EpsilonCore.h"
#include "ui/uiwindow.h"

namespace epsilon
{
	using namespace std;
	using namespace sfg;

	class DebugWindow :
		public UIWindow
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<DebugWindow> Ptr;
		
		static Ptr Create();
		static Ptr Create(string title);

		void Setup(void);
		void OnUpdate(float seconds);

		void OnButtonClick();
		
		explicit DebugWindow(const private_struct &);
		DebugWindow(const private_struct &, string title);

	private:
		Label::Ptr label;
		string	   windowTitle;
	};

}
