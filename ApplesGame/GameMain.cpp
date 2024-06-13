// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"
#include "Apples.h"



int main()
{
	using namespace ApplesGame;
	//Random generation
	int seed = (int)time(nullptr);
	srand(seed);

	//Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples Game!");

	//GAME INITIALIZATION
	Game game;
	InitGame(game);

	//Init clock
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();
	
	//MAIN LOOP
	while (window.isOpen())
	{	
		//Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//READ EVENT
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && CurrentGameState(game) == GameState::Play) {
				if (event.key.code == sf::Keyboard::Escape) {
					SwitchGameState(game, GameState::ExitGame);
				}
			}
			if (event.type == sf::Event::KeyPressed && CurrentGameState(game) == GameState::MainMenu) {
				if (event.key.code == sf::Keyboard::Num1) {
					SwitchGameSettings(game, GameSettingBits::infiniteApples);
				}
				if (event.key.code == sf::Keyboard::Num2) {
					SwitchGameSettings(game, GameSettingBits::accelerationPlayer);
				}
				if (event.key.code == sf::Keyboard::Num3) {
					game.randomNumApples = GetRandomNumber(game, NUM_APPLES);
				}
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Tab) {
					if (CurrentGameState(game) == GameState::MainMenu) {
						SwitchGameState(game, GameState::Leaderboard);
					}
					else if (CurrentGameState(game) == GameState::Leaderboard) {
						SwitchGameState(game, GameState::MainMenu);
					}
				}
			}
			if (event.type == sf::Event::KeyPressed && CurrentGameState(game) == GameState::ExitGame) {
				if (event.key.code == sf::Keyboard::Space) {
					SwitchGameState(game, GameState::Play);
				}
				else if (event.key.code == sf::Keyboard::X) {
					SwitchGameState(game, GameState::MainMenu);
				}
			}
		}

		//UPDATE GAME
		UpdateGame(game, deltaTime);

		//DRAW GAME
		window.clear();
		DrawGame(game, window);
	}

	// DEINITIALIZATION
	Deinitialize(game);

	return 0;
}
