#pragma once

namespace ApplesGame
{
	enum class GameState
	{
		None = 0,
		Init,
		MainMenu,
		Leaderboard,
		Play,
		GameOver,
		GameWin,
		ExitGame
	};

	struct Game;

	void SwitchGameState(Game& game, GameState gameState);
	GameState CurrentGameState(const Game& game);
	void PopGameState(Game& game);
}