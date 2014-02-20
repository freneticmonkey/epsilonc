#pragma once

#include "EpsilonCore.h"
#include "render/Colour.h"
#include <SFML/Graphics.hpp>
#include "math/Vector.h"

namespace epsilon
{
	class Graph
	{
	private:
		struct private_struct {};
		
	public:
		typedef std::shared_ptr<Graph> Ptr;

		static Ptr Create(std::string name);

		~Graph();

		void OnUpdate(float seconds)
		{

		}

		std::string GetName()
		{
			return name;
		}

		void Draw(sf::RenderWindow * window);

		void SetColour(Colour theColour);

		void AddValue(float value);

		explicit Graph(const private_struct &, std::string graphName);
		//Graph(const private_struct &, std::string title);
		
	private:
		std::string		name;
		sf::VertexArray points;
		sf::Color		colour;
		unsigned int	currentPoint;
		Vector2			screenPos;
		Vector2			screenDim;
	};

}
