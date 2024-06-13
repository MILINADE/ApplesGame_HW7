#include "Game.h"
#include <algorithm>
#include <cassert>

namespace ApplesGame 
{
	bool HasGameSettingsSetting(Game& game, GameSettingBits settings)
	{
		return static_cast<int>(game.gameMode) & static_cast<int>(settings);
	}

	void SwitchGameSettings(Game& game, GameSettingBits settings)
	{
		game.gameMode = static_cast<GameSettingBits>((static_cast<int>(game.gameMode) ^ static_cast<int>(settings)));
	}

	bool CompareByScoreDescending(PlayerData& a, PlayerData& b)
	{
		return a.score > b.score;
	}

	int GetRandomNumber(Game& game, int maxSize)
	{
		return (int)rand() % maxSize;
	}

	void InitPlayersTable(Game& game)
	{
		game.playersData.resize(PLAYERS_NUM);

		for (size_t i = 0; i < PLAYERS_NUM; i++)
		{
			game.playersData[i].name = playersNameStrings[rand() % PLAYERS_NUM];
			game.playersData[i].score = rand() % 25;
		}

		game.playersData[4].name = "Player";
		game.playersData[4].score = game.totalNumberEatenApplesByPlayer;
	}

	void StartMainMenuState(Game& game)
	{
		//UI
		sf::FloatRect applesGameStartRect = game.applesGameStartText.getLocalBounds();
		/*game.applesGameStartText.setOrigin(applesGameStartRect.left + applesGameStartRect.width / 2.f,
			applesGameStartRect.top + applesGameStartRect.height / 2.f);*///268.000000, 51.5000000
		game.applesGameStartText.setOrigin(268.f, 51.5f);
		game.applesGameStartText.setPosition(SCREEN_WIDTH / 2.f, (SCREEN_HEIGHT / 2.f) - ((SCREEN_HEIGHT / 2.f) / 2.f));
		game.applesGameStartText.setString("APPLES GAME");

		game.background.setFillColor(sf::Color::Green);

		SetSpriteRelativeOrigin(game.logoSprite, 0.5f, 0.5f);
		game.logoSprite.setScale(0.2f, 0.2f);
		game.logoSprite.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		SwitchGameState(game, GameState::MainMenu);
	}

	void UpdateMainMenuState(Game& game)
	{
		game.numApplesText.setString("Apples in game(num3 to random): " + std::to_string(game.randomNumApples));
		game.spaceToStartText.setString("Press space to start\nPress escape to exit");

		if (HasGameSettingsSetting(game, GameSettingBits::infiniteApples)) {
			game.infiniteApplesText.setString("Infinite Apples(num1 to switch): on");
		}
		else {
			game.infiniteApplesText.setString("Infinite Apples(num1 to switch): off");
		}
		if (HasGameSettingsSetting(game, GameSettingBits::accelerationPlayer)) {
			game.accelerationPlayerText.setString("Acceleration player(num2 to switch): on");
		}
		else{
			game.accelerationPlayerText.setString("Acceleration player(num2 to switch): off");
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			game.background.setFillColor(sf::Color::Black);
			StartPlayingState(game);
		}
	}

	void StartLeaderboardState(Game& game)
	{
		sf::FloatRect textRect = game.leaderboard.getLocalBounds();
		game.leaderboard.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
		game.leaderboard.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 100.f);
		
		game.background.setFillColor(sf::Color::Black);

		for (size_t i = 0; i < PLAYERS_NUM; i++)
		{
			if (game.playersData[i].name == "Player") {
				game.playersData[i].score = game.totalNumberEatenApplesByPlayer;
			}
		}

		std::sort(game.playersData.begin(), game.playersData.begin() + PLAYERS_NUM, CompareByScoreDescending);

		game.leaderboard.setString("Leaderboard");
		for (size_t i = 0; i < PLAYERS_NUM; i++)
		{
			game.playerNameText[i].setString(std::to_string(i + 1) + ". " + game.playersData[i].name + ": " + std::to_string(game.playersData[i].score));
		}
		UpdateLeaderboardState(game);
	}

	void UpdateLeaderboardState(Game& game)
	{
		
	}

	void StartPlayingState(Game& game)
	{
		//Init Player
		SetPlayerPosition(game.player, { SCREEN_WIDTH / 2.f,SCREEN_HEIGHT / 2.f });
		SetPlayerSpeed(game.player, INITIAL_SPEED);
		SetPlayerDirection(game.player, PlayerDirection::Right);
		InitPlayer(game.player, game);
		//Init Apples
		for (size_t i = 0; i < game.apples.size(); i++)
		{
			if (i < game.randomNumApples) {
				game.apples[i].isAppleEaten = false;
				SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
			}
			else if (i > game.randomNumApples) {
				game.apples[i].isAppleEaten = true;
			}
		}

		//Init Rocks
		for (size_t i = 0; i < NUM_ROCKS; i++) {
			SetRockPosition(game.rockV[i], GetRandomPositionInRectangle(game.screenRect));
		}

		game.timeSinceGameFinish = 0.f;
		game.gameFinishTime = 0.f;
		game.numEatenApples = 0;
		game.scoreText.setString("Eaten apples:" + std::to_string(game.numEatenApples));
		SwitchGameState(game, GameState::Play);
	}

	void UpdatePlayingState(Game& game, float deltaTime)
	{
		//Read Keyboard input to player direction (For infinite move to direction)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			SetPlayerDirection(game.player, PlayerDirection::Right);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			SetPlayerDirection(game.player, PlayerDirection::Up);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			SetPlayerDirection(game.player, PlayerDirection::Left);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			SetPlayerDirection(game.player, PlayerDirection::Down);
		}

		//Calculate player speed acceleration
		//game.player.speed += deltaTime * ACCELERATION;

		//Read direction and calculate speed
		UpdatePlayer(game.player, deltaTime);

		//Calculate boards collision
		if (!DoShapeCollide(GetPlayerCollider(game.player), game.screenRect)) {
			StartGameOverState(game);
		}

		//Player collision to apples and game settings check
		for (auto& apple : game.apples)
		{
			if (!apple.isAppleEaten && DoShapeCollide(GetPlayerCollider(game.player), GetAppleCollider(apple))) {
				apple.isAppleEaten = true;
				game.numEatenApples++;
				if (HasGameSettingsSetting(game, GameSettingBits::infiniteApples)) {
					SetApplePosition(apple, GetRandomPositionInRectangle(game.screenRect));
					apple.isAppleEaten = false;
				}
				if (!HasGameSettingsSetting(game, GameSettingBits::infiniteApples) && game.randomNumApples == game.numEatenApples)
				{
					StartWinGameState(game);
				}
				if (HasGameSettingsSetting(game, GameSettingBits::accelerationPlayer)) {
					SetPlayerSpeed(game.player, GetPlayerSpeed(game.player) + ACCELERATION);
				}
				game.eatAppleSound.play();
				game.scoreText.setString("Eaten apples:" + std::to_string(game.numEatenApples));

			}
		}

		//Player collision to rocks
		for (size_t i = 0; i < NUM_ROCKS; i++)
		{
			if (DoShapeCollide(GetPlayerCollider(game.player), GetRockCollider(game.rockV[i])))
			{
				StartGameOverState(game);
			}
		}
	}

	void StartGameOverState(Game& game)
	{
		game.totalNumberEatenApplesByPlayer += game.numEatenApples;
		game.gameFinishTime = game.timeSinceGameFinish;
		game.endGameSound.play();
		SwitchGameState(game, GameState::GameOver);
	}

	void UpdateGameOverState(Game& game, float deltaTime)
	{
		if (game.timeSinceGameFinish <= PAUSE_LENGHT) {
			game.timeSinceGameFinish += deltaTime;
			game.background.setFillColor(sf::Color::Red);
		}
		//Reset game
		else {
			game.background.setFillColor(sf::Color::Black);
			StartMainMenuState(game);
			//StartPlayingState(game);
		}
	}

	void StartWinGameState(Game& game)
	{
		game.totalNumberEatenApplesByPlayer += game.numEatenApples;
		game.gameFinishTime = game.timeSinceGameFinish;
		game.winGameSound.play();
		SwitchGameState(game, GameState::GameWin);
	}

	void UpdateWinGameState(Game& game, float deltaTime)
	{
		if (game.timeSinceGameFinish <= PAUSE_LENGHT) {
			game.timeSinceGameFinish += deltaTime;
			game.background.setFillColor(sf::Color::Red);
		}
		//Reset game
		else {
			game.background.setFillColor(sf::Color::Black);
			StartMainMenuState(game);
			//StartPlayingState(game);
		}
	}

	void InitGame(Game& game) {
		SwitchGameState(game, GameState::Init);

		game.apples.resize(NUM_APPLES);
		game.rockV.resize(NUM_ROCKS);

		InitPlayersTable(game);

		game.screenRect = { 0.f,0.f, SCREEN_WIDTH,SCREEN_HEIGHT };
		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
		assert(game.logoTexture.loadFromFile(RESOURCES_PATH + "\\logo.png"));

		assert(game.eatAppleBuffer.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
		assert(game.endGameBuffer.loadFromFile(RESOURCES_PATH + "\\Death.wav"));
		assert(game.winGameBuffer.loadFromFile(RESOURCES_PATH + "\\WinGame.wav"));
		
		assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Black.ttf"));

		for (auto& apples: game.apples)
		{
			InitApples(apples, game);
		}

		for (size_t i = 0; i < NUM_ROCKS; i++)
		{
			InitRock(game.rockV[i], game);
		}

		game.logoSprite.setTexture(game.logoTexture);

		game.eatAppleSound.setBuffer(game.eatAppleBuffer);
		game.endGameSound.setBuffer(game.endGameBuffer);
		game.winGameSound.setBuffer(game.winGameBuffer);

		game.numApplesText.setFont(game.font);
		game.numApplesText.setCharacterSize(20);
		game.numApplesText.setFillColor(sf::Color::White);
		game.numApplesText.setPosition(0.f, 0.f);

		game.applesGameStartText.setFont(game.font);
		game.applesGameStartText.setCharacterSize(80);
		game.applesGameStartText.setFillColor(sf::Color::White);
		
		
		game.leaderboard.setFont(game.font);
		game.leaderboard.setCharacterSize(40);
		game.leaderboard.setFillColor(sf::Color::White);
		

		for (size_t i = 0; i < PLAYERS_NUM; i++)
		{
			game.playerNameText[i].setFont(game.font);
			game.playerNameText[i].setCharacterSize(20);
			game.playerNameText[i].setFillColor(sf::Color::White);
			game.playerNameText[i].setPosition(SCREEN_WIDTH / 2.f - 80.f, SCREEN_HEIGHT / 2.f - 50.f + i * 40);
		}
		
		game.infiniteApplesText.setFont(game.font);
		game.infiniteApplesText.setCharacterSize(20);
		game.infiniteApplesText.setFillColor(sf::Color::White);
		game.infiniteApplesText.setPosition(SCREEN_WIDTH / 2.f - SCREEN_WIDTH / 2.2f, SCREEN_HEIGHT / 2.f + 200.f);

		game.accelerationPlayerText.setFont(game.font);
		game.accelerationPlayerText.setCharacterSize(20);
		game.accelerationPlayerText.setFillColor(sf::Color::White);
		game.accelerationPlayerText.setPosition(SCREEN_WIDTH / 2.f - SCREEN_WIDTH / 2.2f, SCREEN_HEIGHT / 2.f + 20.f + 200.f);
		
		game.spaceToStartText.setFont(game.font);
		game.spaceToStartText.setCharacterSize(20);
		game.spaceToStartText.setFillColor(sf::Color::White);
		game.spaceToStartText.setPosition(SCREEN_WIDTH - 200.f, 0.f);

		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(20);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(0.f, 0.f);

		game.howToControlText.setFont(game.font);
		game.howToControlText.setCharacterSize(20);
		game.howToControlText.setFillColor(sf::Color::White);
		game.howToControlText.setPosition(SCREEN_WIDTH - 190.f, 0.f);
		game.howToControlText.setString("Use arrows to move");

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(100);
		game.gameOverText.setFillColor(sf::Color::White);

		game.winGameText.setFont(game.font);
		game.winGameText.setCharacterSize(100);
		game.winGameText.setFillColor(sf::Color::White);

		game.exitGameText.setFont(game.font);
		game.exitGameText.setCharacterSize(30);
		game.exitGameText.setFillColor(sf::Color::White);

		StartMainMenuState(game);
	}

	void UpdateGame(Game& game, float deltaTime) {
		GameState currentGameState = CurrentGameState(game);
		switch (currentGameState)
		{
		case ApplesGame::GameState::MainMenu:
			UpdateMainMenuState(game);
			break;
		case ApplesGame::GameState::Leaderboard:
			StartLeaderboardState(game);
			break;
		case ApplesGame::GameState::Play:
			UpdatePlayingState(game, deltaTime);
			break;
		case ApplesGame::GameState::GameOver:
			UpdateGameOverState(game, deltaTime);
			break;
		case ApplesGame::GameState::GameWin:
			UpdateWinGameState(game, deltaTime);
			break;
		case ApplesGame::GameState::ExitGame:
			UpdateExitGame(game);
			break;
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window) {
		window.draw(game.background);
		
		DrawPlayer(game.player, window);
		
		for (auto& apples : game.apples) {
			if (!apples.isAppleEaten) {
				DrawApples(apples, window);
			}
		}

		for (size_t i = 0; i < NUM_ROCKS; i++)
		{
			DrawRock(game.rockV[i],window);
		}

		if (CurrentGameState(game) == GameState::GameWin) {
			sf::FloatRect winGameTextRect = game.winGameText.getLocalBounds();
			game.winGameText.setOrigin(winGameTextRect.left + winGameTextRect.width / 2.f,
				winGameTextRect.top + winGameTextRect.height / 2.f);
			game.winGameText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
			game.winGameText.setString("GAME WIN");
			window.draw(game.winGameText);
		}

		if (CurrentGameState(game) == GameState::GameOver) {
			sf::FloatRect gameOverTextRect = game.gameOverText.getLocalBounds();
			game.gameOverText.setOrigin(gameOverTextRect.left + gameOverTextRect.width / 2.f,
				gameOverTextRect.top + gameOverTextRect.height / 2.f);//268.000000, 51.5000000
			game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
			game.gameOverText.setString("GAME OVER");
			window.draw(game.gameOverText);
		}
		else if (CurrentGameState(game) == GameState::Play) {
			window.draw(game.scoreText);
			window.draw(game.howToControlText);
		}
		else if (CurrentGameState(game) == GameState::MainMenu)
		{
			window.clear();
			window.draw(game.applesGameStartText);
			window.draw(game.infiniteApplesText);
			window.draw(game.accelerationPlayerText);
			window.draw(game.spaceToStartText);
			window.draw(game.numApplesText);
			window.draw(game.logoSprite);
		}
		else if (CurrentGameState(game) == GameState::Leaderboard) {
			window.draw(game.leaderboard);
			for (size_t i = 0; i < PLAYERS_NUM; i++)
			{
				window.draw(game.playerNameText[i]);
			}
			window.draw(game.applesGameStartText);
			game.spaceToStartText.setString("Press Tab to exit\nto main menu");
			window.draw(game.spaceToStartText);
		}
		else if (CurrentGameState(game) == GameState::ExitGame) {
			game.exitGameText.setPosition(SCREEN_WIDTH / 2.f - 270.f, SCREEN_HEIGHT / 2.f - 50);
			game.exitGameText.setString("                              Pause\nspace - continue / x - exit to main menu");
			window.draw(game.exitGameText);
		}
		window.display();
	}

	void Deinitialize(Game& game) {

	}
	
	void StartExitGameState(Game& game){
		SwitchGameState(game, GameState::ExitGame);
	}

	void UpdateExitGame(Game& game)
	{
		//Read keyboard
	}


}

