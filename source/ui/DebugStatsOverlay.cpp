#include "ui/DebugStatsOverlay.h"
#include <algorithm>

namespace epsilon
{
	const float DebugStatsOverlay::THIRTY_FPS = 33.333f;
	const float DebugStatsOverlay::SIXTY_FPS  = 16.667f;

	DebugStatsOverlay::Ptr DebugStatsOverlay::Create()
	{
		return std::make_shared<DebugStatsOverlay>(private_struct());
	}

	DebugStatsOverlay::DebugStatsOverlay(const private_struct &) : UIOverlay("debug_stats"),
																   border(sf::LinesStrip, 12),
																   screenPos(5,400),
																   dimensions(295,100),
																   scale(2.0f)
	{
		
	}

	DebugStatsOverlay::~DebugStatsOverlay()
	{

	}

	void DebugStatsOverlay::Setup()
	{
		// Setup the background
		background.setPosition(screenPos);
		background.setSize(dimensions);
		background.setFillColor(sf::Color(255, 255, 255, 100));

		// Draw border

		// Top Left
		border[0].position	= screenPos;
		border[0].color		= sf::Color(255, 0, 0, 255);
		// TR
		border[1].position	= sf::Vector2f(screenPos.x + dimensions.x, screenPos.y);
		border[1].color		= sf::Color(255, 0, 0, 255);
		// BR
		border[2].position  = sf::Vector2f(screenPos.x + dimensions.x, screenPos.y + dimensions.y);
		border[2].color		= sf::Color(255, 0, 0, 255);
		// BL
		border[3].position  = sf::Vector2f(screenPos.x, screenPos.y + dimensions.y);
		border[3].color		= sf::Color(255, 0, 0, 255);

		border[4].position = screenPos;
		border[4].color = sf::Color(255, 0, 0, 255);

		// Border
		
		// 30fps
		border[5].position = sf::Vector2f(screenPos.x, screenPos.y + (dimensions.y - (THIRTY_FPS * scale)));
		border[5].color = sf::Color(255, 0, 0, 255);

		border[6].position = sf::Vector2f(screenPos.x, screenPos.y + (dimensions.y - (THIRTY_FPS * scale) ));
		border[6].color = sf::Color(255, 168, 0, 150);
		
		border[7].position = sf::Vector2f(screenPos.x + dimensions.x, screenPos.y + (dimensions.y - (THIRTY_FPS* scale) ));
		border[7].color = sf::Color(255, 168, 0, 150);

		border[8].position = sf::Vector2f(screenPos.x + dimensions.x, screenPos.y + (dimensions.y - (THIRTY_FPS* scale) ));
		border[8].color = sf::Color(255, 0, 0, 255);
		
		// 60fps
		border[9].position = sf::Vector2f(screenPos.x + dimensions.x, screenPos.y + (dimensions.y - (SIXTY_FPS * scale) ));
		border[9].color = sf::Color(255, 0, 0, 255);

		border[10].position = sf::Vector2f(screenPos.x + dimensions.x, screenPos.y + (dimensions.y - (SIXTY_FPS * scale)));
		border[10].color = sf::Color(0, 255, 0, 150);

		border[11].position = sf::Vector2f(screenPos.x, screenPos.y + (dimensions.y - (SIXTY_FPS * scale) ));
		border[11].color = sf::Color(0, 255, 0, 150);

	}

	void DebugStatsOverlay::OnUpdate(float seconds)
	{

	}

	Graph::Ptr DebugStatsOverlay::CreateGraph(std::string name)
	{
		Graph::Ptr newGraph = Graph::Create(name);
		newGraph->SetPosition(screenPos)
				->SetScale(scale);
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