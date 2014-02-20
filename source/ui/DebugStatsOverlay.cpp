#include "ui/DebugStatsOverlay.h"
#include <algorithm>

namespace epsilon
{
	DebugStatsOverlay::Ptr DebugStatsOverlay::Create()
	{
		return std::make_shared<DebugStatsOverlay>(private_struct());
	}

	DebugStatsOverlay::DebugStatsOverlay(const private_struct &) : UIOverlay("debug_stats"),
																   border(sf::LinesStrip, 2)
	{
		
	}

	DebugStatsOverlay::~DebugStatsOverlay()
	{

	}

	void DebugStatsOverlay::Setup()
	{
		// Setup the background
		background.setPosition(sf::Vector2f(5, 400));
		background.setSize(sf::Vector2f(300, 100));
		background.setFillColor(sf::Color(255, 255, 255, 100));

		border[0].position = sf::Vector2f(5, 500);
		border[0].color = sf::Color(255, 255, 255, 255);
		border[1].position = sf::Vector2f(305, 500);
		border[1].color = sf::Color(255, 255, 255, 255);
	}

	void DebugStatsOverlay::OnUpdate(float seconds)
	{

	}

	Graph::Ptr DebugStatsOverlay::CreateGraph(std::string name)
	{
		Graph::Ptr newGraph = Graph::Create(name);
		graphs.push_back(newGraph);

		return newGraph;
	}

	void DebugStatsOverlay::Draw(sf::RenderWindow * window)
	{
		// Finally draw graphs. Preferably on top of the rest of the UI
		window->resetGLStates();

		// Draw Background first
		window->draw(background);

		// Draw Baseline
		window->draw(border);

		// Draw each of the graphs
		for_each(graphs.begin(), graphs.end(), [window](Graph::Ptr child){
			child->Draw(window);
		});
	}

	void DebugStatsOverlay::AddGraphValue(std::string name, float value)
	{
		GraphList::iterator targetGraph = find_if(graphs.begin(), graphs.end(), [name](Graph::Ptr child){
			return child->GetName() == name;
		});

		if ( !(targetGraph == graphs.end()) )
		{
			(*targetGraph)->AddValue(value);
		}
	}
}