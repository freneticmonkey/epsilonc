#include "ui/Graph.h"

namespace epsilon
{
	Graph::Ptr Graph::Create(std::string name)
	{
		return std::make_shared<Graph>(private_struct(), name);
	}

	Graph::Graph(const private_struct &, std::string graphName) : points(sf::LinesStrip, 100),
																  currentPoint(0),
																  name(graphName),
																  colour(255,0,0,255),
																  screenPos(5, 400),
																  screenDim(300,100),
																  scale(1.0f),
																  initialFill(false)
	{
		// Setup the graph
		for (unsigned int i = 0; i < points.getVertexCount(); i++)
		{
			points[i].position = sf::Vector2f(screenPos.x + (i * 3), (screenPos.y + screenDim.y));
		}
	}

	Graph::~Graph()
	{

	}

	Graph::Ptr Graph::SetPosition(sf::Vector2f pos)
	{
		screenPos = Vector2(pos.x, pos.y);
		return ThisPtr();
	}

	Graph::Ptr Graph::SetColour(Colour theColour)
	{
		theColour.To8bit();
		colour.r = theColour.r;
		colour.g = theColour.g;
		colour.b = theColour.b;
		colour.a = theColour.a;

		return ThisPtr();
	}

	void Graph::AddValue(float value)
	{
		// Adjust the value based on the screen position
		float pointX = screenPos.x;
		float pointY = (screenPos.y + screenDim.y) - (value * scale);

		if (currentPoint < (points.getVertexCount() - 1))
		{
			initialFill = true;

			// Just setup the new point
			points[currentPoint].position = sf::Vector2f(pointX + currentPoint * 3, pointY);
			points[currentPoint].color = colour;

			// Setup the bottom point
			points[currentPoint + 1].position = sf::Vector2f(pointX + currentPoint * 3, pointY);
			points[currentPoint + 1].color = colour;

			currentPoint++;

		}
		else
		{
			initialFill = false;
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

	bool Graph::SetScale(float newScale)
	{
		bool success = false;
		
		// If the graph is not in the process of being populated with an initial 
		// set of values, then the scale can be adjusted.
		// This prevents a graph containing points that have been scaled differently.
		if ( !initialFill )
		{
			// If the initial data has been set, then the existing graph values need to be scaled.
			// If currentPoint is 0, then no values have been yet entered into the graph
			if (currentPoint > 0)
			{
				// Reduce the existing points to a 1.0f scale
				if (scale != 1.0f)
				{
					for (unsigned int i = 0; i < points.getVertexCount(); i++)
					{
						points[i].position.y = points[i].position.y * (1.0f / scale);
					}
				}

				// Scale them for the new scale
				for (unsigned int i = 0; i < points.getVertexCount(); i++)
				{
					points[i].position.y = points[i].position.y * newScale;
				}
			}

			// Update the scale value
			scale = newScale;
			success = true;
		}

		return success;
	}

	void Graph::Draw(sf::RenderWindow * window)
	{
		window->draw(points);
	}
}