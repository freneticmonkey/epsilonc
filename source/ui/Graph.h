#pragma once

#include "EpsilonCore.h"
#include "render/Colour.h"
#include <SFML/Graphics.hpp>
#include "math/Vector.h"

namespace epsilon
{
	class Graph : 
		public std::enable_shared_from_this<Graph>
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

		Graph::Ptr ThisPtr() { return shared_from_this(); }

		void Draw(sf::RenderWindow * window);
	
		Graph::Ptr SetPosition(sf::Vector2f pos);
		Graph::Ptr SetColour(Colour theColour);
		bool SetScale(float newScale);

		void AddValue(float value);

		
		explicit Graph(const private_struct &, std::string graphName);
		//Graph(const private_struct &, std::string title);
		
	private:
		std::string		name;
		sf::VertexArray points;
		unsigned int	currentPoint;
		sf::Color		colour;
		float			scale;
		bool			initialFill;		

		Vector2			screenPos;
		Vector2			screenDim;
	};

}
