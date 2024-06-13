#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace ApplesGame 
{
	enum class PlayerDirection : int
	{
		Right = 0,
		Up,
		Left,
		Down
	};

	struct Game;

	struct Player
	{
		//Player Data
		Position2D position;
		float speed = INITIAL_SPEED;
		PlayerDirection direction = PlayerDirection::Right;
		sf::Sprite sprite;
	};

	void InitPlayer(Player& player, const Game& game);

	void SetPlayerDirection(Player& player, PlayerDirection direction);
	void SetPlayerPosition(Player& player, const Position2D position);
	void SetPlayerSpeed(Player& player, float speed);
	float GetPlayerSpeed(const Player& player);
	Rectangle GetPlayerCollider(const Player& player);
	void UpdatePlayer(Player& player, float deltaTime);
	
	void DrawPlayer(Player& player, sf::RenderWindow& window);
}