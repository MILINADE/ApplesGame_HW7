#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "Math.h"

namespace ApplesGame 
{
	bool DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2)
	{
		return rect1.position.x < rect2.position.x + rect2.size.x &&
			rect1.position.x + rect1.size.x > rect2.position.x &&
			rect1.position.y < rect2.position.y + rect2.size.y &&
			rect1.position.y + rect1.size.y > rect2.position.y;
	}
	bool DoShapeCollide(const Circle& circle1, const Circle& circle2)
	{
		float dx = (float)fabs(circle1.position.x - circle2.position.x);
		float dy = (float)fabs(circle1.position.y - circle2.position.y);
		float distance = sqrt(dx * dx + dy * dy);
		return distance < circle1.radius + circle2.radius;
	}
	bool DoShapeCollide(const Rectangle& rect1, const Circle& circle2)
	{
		float dx = circle2.position.x - std::max(rect1.position.x, std::min(circle2.position.x, rect1.position.x + rect1.size.x));
		float dy = circle2.position.y - std::max(rect1.position.y, std::min(circle2.position.y, rect1.position.y + rect1.size.y));
		return (dx * dx + dy * dy) < (circle2.radius * circle2.radius);
	}

	Position2D GetRandomPositionInRectangle(const Rectangle& rect)
	{
		Position2D result;
		result.x = rand() / (float)RAND_MAX * rect.size.x + rect.position.x;
		result.y = rand() / (float)RAND_MAX * rect.size.y + rect.position.y;
		return result;
	}

	bool IsRectanglesCollide(Position2D rect1Position, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size)
	{
		float dx = (float)fabs(rect1Position.x - rect2Position.x);
		float dy = (float)fabs(rect1Position.y - rect2Position.y);
		return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
			dy <= (rect1Size.y + rect2Size.y) / 2.f);
	}

	bool IsCirclesCollide(Position2D rectPosition, Vector2D rectSize, Position2D circlePosition, Vector2D circleSize) {
		float squareDistance = (rectPosition.x - circlePosition.x) * (rectPosition.x - circlePosition.x) +
			(rectPosition.y - circlePosition.y) * (rectPosition.y - circlePosition.y);
		float squareRadiusSum = (rectSize.x + circleSize.x) * (rectSize.y + circleSize.y) / 4.f;
		return squareDistance <= squareRadiusSum;
	}
	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight){
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = { desiredWidth / spriteRect.width, desiredHeight / spriteRect.height };
		sprite.setScale(scale);
	}

	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
	}
		
}