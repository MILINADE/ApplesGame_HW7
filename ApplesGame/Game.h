#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Apples.h"
#include "Rock.h"
#include "GameState.h"
#include <SFML/Audio.hpp>

namespace ApplesGame 
{
	enum class GameSettingBits
	{
		infiniteApples = 1 << 0,
		accelerationPlayer = 1 << 1,
	};

	const std::string playersNameStrings[PLAYERS_NUM] = {
	"JOHN", "EMMA", "MICHAEL", "OLIVIA", "WILLIAM"
	};

	struct PlayerData
	{
		std::string name;
		int score = 0;
	};

	struct Game
	{
		//Player
		Player player;
		//Apples Data
		std::vector <Apples> apples;
		//Rock Data
		std::vector <Rock> rockV;

		//Global game data
		std::vector <GameState> gameStateStack;
		/*bool isGameFinished = false;
		bool isGameWin = false;
		bool isMainMenuState = true;*/
		float gameFinishTime = 0.f;
		float timeSinceGameFinish = 0.f;
		int randomNumApples = rand() % NUM_APPLES;

		Rectangle screenRect;

		GameSettingBits gameMode = GameSettingBits::infiniteApples;

		//Player data
		std::vector<PlayerData> playersData;

		//Scores
		int numEatenApples = 0;
		int totalNumberEatenApplesByPlayer = 0;

		//Resourses
		sf::RectangleShape background;
		
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::Texture logoTexture;

		sf::SoundBuffer eatAppleBuffer;
		sf::Sound eatAppleSound;
		sf::SoundBuffer endGameBuffer;
		sf::Sound endGameSound;
		sf::SoundBuffer winGameBuffer;
		sf::Sound winGameSound;

		//UI
		sf::Font font;

		sf::Text numApplesText;

		sf::Text applesGameStartText;
		sf::Text infiniteApplesText;
		sf::Text accelerationPlayerText;
		sf::Text spaceToStartText;

		sf::Text leaderboard;
		sf::Text playerNameText[PLAYERS_NUM];
		
		sf::Text scoreText;
		sf::Text howToControlText;

		sf::Text gameOverText;
		sf::Text winGameText;

		sf::Text exitGameText;

		sf::Sprite logoSprite;
	};

	//void RestartGame(Game& game);
	bool HasGameSettingsSetting(Game& game, GameSettingBits settings);
	void SwitchGameSettings(Game& game, GameSettingBits settings);
	bool CompareByScoreDescending(PlayerData& a, PlayerData& b);
	
	int GetRandomNumber(Game& game, int maxSize);
	void InitPlayersTable(Game& game);

	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void Deinitialize(Game& game);

	void StartMainMenuState(Game& game);
	void UpdateMainMenuState(Game& game);

	void StartLeaderboardState(Game& game);
	void UpdateLeaderboardState(Game& game);

	void StartPlayingState(Game& game);
	void UpdatePlayingState(Game& game, float deltaTime);

	void StartGameOverState(Game& game);
	void UpdateGameOverState(Game& game, float deltaTime);

	void StartWinGameState(Game& game);
	void UpdateWinGameState(Game& game, float deltaTime);

	void StartExitGameState(Game& game);
	void UpdateExitGame(Game& game);
}

