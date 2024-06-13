#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"

namespace ApplesGame 
{
	struct Game;

	struct Apples
	{
		Position2D position;
		bool isAppleEaten = false;
		sf::Sprite sprite;
	};


	void InitApples(Apples& apples, const Game& game);

	void DrawApples(Apples& apples, sf::RenderWindow& window);

	void SetApplePosition(Apples& apple,const Position2D& position);

	Circle GetAppleCollider(const Apples& apple);

}

