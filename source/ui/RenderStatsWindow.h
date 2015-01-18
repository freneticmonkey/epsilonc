#pragma once

#include "EpsilonCore.h"
#include "ui/UIWindow.h"

namespace epsilon
{
	class RenderStatsWindow :
		public UIWindow
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<RenderStatsWindow> Ptr;

		static Ptr Create();

		void Setup(void);
		void OnGUI(float seconds);
		void Destroy(void);

		void SetFPS(float iFPS) { fps = iFPS; }

		explicit RenderStatsWindow(const private_struct &);

	private:
		float fps;
	};

}
