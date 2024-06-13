#include "Apples.h"
#include "Constants.h"
#include "Game.h"

namespace ApplesGame 
{
	void InitApples(Apples& apples, const Game& game) {
		apples.sprite.setTexture(game.appleTexture);
		SetSpriteSize(apples.sprite, APPLES_SIZE, APPLES_SIZE);
		SetSpriteRelativeOrigin(apples.sprite, 0.5f, 0.5f);
	}

	void DrawApples(Apples& apples, sf::RenderWindow& window)
	{
		apples.sprite.setPosition(apples.position.x, apples.position.y);
		window.draw(apples.sprite);
	}

	void SetApplePosition(Apples& apple, const Position2D& position)
	{
		apple.position = position;
	}

	Circle GetAppleCollider(const Apples& apple)
	{
		return {apple.position,APPLES_SIZE/2.f};
	}

}

