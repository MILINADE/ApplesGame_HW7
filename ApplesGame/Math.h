#pragma once

namespace sf {
	class Sprite;
}

namespace ApplesGame 
{
	struct Vector2D {
		float x = 0.f;
		float y = 0.f;
	};

	typedef Vector2D Position2D;

	struct Rectangle
	{
		Position2D position;
		Vector2D size;
	};

	struct Circle
	{
		Position2D position;
		float radius;
	};


	bool DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2);
	bool DoShapeCollide(const Circle& circle1, const Circle& circle2);
	bool DoShapeCollide(const Rectangle& rect1, const Circle& circle2);

	Position2D GetRandomPositionInRectangle(const Rectangle& rect);
	bool IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size,
							 Position2D rect2Position, Vector2D rect2Size);
	bool IsCirclesCollide(Position2D rectPosition, Vector2D rectSize, Position2D circlePosition, Vector2D circleSize);
	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);
	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY);
}