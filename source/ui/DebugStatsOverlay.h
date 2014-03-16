#pragma once

#include "EpsilonCore.h"

#include "render/Colour.h"
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

#include "ui/UIOverlay.h"
#include "ui/Graph.h"

namespace epsilon
{
	class DebugStatsOverlay :
		public UIOverlay
	{
	private:
		struct private_struct {};

		typedef std::list<Graph::Ptr> GraphList;

	public:
		typedef std::shared_ptr<DebugStatsOverlay> Ptr;

		static Ptr Create();

		~DebugStatsOverlay();

		void Setup();

		void OnUpdate(float seconds);

		void Draw(sf::RenderWindow * window);

		Graph::Ptr CreateGraph(std::string name);

		void AddGraphValue(std::string name, float value);

		explicit DebugStatsOverlay(const private_struct &);

	private:
		GraphList			graphs;
		sf::Vector2f		screenPos;
		sf::Vector2f		dimensions;

		sf::VertexArray		border;
		sf::RectangleShape  background;
		float				scale;

		static const float  THIRTY_FPS;
		static const float  SIXTY_FPS;
	};

}
