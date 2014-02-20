#include "ui/Graph.h"

namespace epsilon
{
	Graph::Ptr Graph::Create(std::string name)
	{
		return std::make_shared<Graph>(private_struct(), name);
	}

	Graph::Graph(const private_struct &, std::string graphName) : points(sf::LinesStrip, 100),
																  currentPoint(1),
																  name(graphName),
																  colour(255,0,0,255),
																  screenPos(5, 400),
																  screenDim(300,100)
	{
		// Setup the graph
		for (unsigned int i = 0; i < points.getVertexCount(); i++)
		{
			points[i].position = sf::Vector2f(i*2, 400);
		}
	}

	Graph::~Graph()
	{

	}

	void Graph::SetColour(Colour theColour)
	{
		theColour.To8bit();
		colour.r = theColour.r;
		colour.g = theColour.g;
		colour.b = theColour.b;
		colour.a = theColour.a;
	}

	void Graph::AddValue(float value)
	{
		// Adjust the value based on the screen position
		float pointX = screenPos.x;
		float pointY = (screenPos.y + screenDim.y) - value;

		if (currentPoint < (points.getVertexCount() - 2))
		{
			// Just setup the new point
			points[currentPoint].position = sf::Vector2f(pointX + currentPoint * 3, pointY);
			points[currentPoint].color = colour;

			currentPoint++;

			// Setup the bottom point
			points[currentPoint + 1].position = sf::Vector2f(pointX + currentPoint * 3, pointY);
			points[currentPoint + 1].color = colour;
		}
		else
		{
			// otherwise move all the existing points back one
			for (unsigned int i = 1; i <= currentPoint; i++)
			{
				points[i - 1].position.y = points[i].position.y;
			}
			// Now set the last point
			points[currentPoint].position.y = pointY;
			points[points.getVertexCount()-1].position.y = pointY;
		}
	}

	void Graph::Draw(sf::RenderWindow * window)
	{
		window->draw(points);
	}
}