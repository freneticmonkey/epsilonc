#pragma once

#include "EpsilonCore.h"
#include "math/Vector.h"

namespace epsilon
{
	// This is duplicate code for now.  This should be abstracted 
	// into a parent class for UIOverlay and UIWindow

	class UIOverlay
	{
	public:

		UIOverlay(std::string overlayName) : name(overlayName),
											 visible(true)
		{}

		typedef std::shared_ptr<UIOverlay> Ptr;

		virtual ~UIOverlay() {};

		virtual void Setup(void) = 0;

		virtual void OnUpdate(float seconds) = 0;

		virtual void Draw(sf::RenderWindow * window) = 0;

		std::string GetName() { return name; }

		// Overlay Operations
		void Show(bool show)
		{
			visible = show;
		}

		bool IsVisible()
		{
			return visible;
		}

		void SetPosition(Vector2 newPosition)
		{
			position = newPosition;
		}

		Vector2 GetPosition()
		{
			return position;
		}

	protected:
		std::string name;
		Vector2     position;
		Vector2		size;
		bool		visible;
	};

}