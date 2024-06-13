#include "GameState.h"
#include "Game.h"

namespace ApplesGame 
{
	void SwitchGameState(Game& game, GameState gameState) {
		game.gameStateStack.push_back(gameState);
	}

	GameState CurrentGameState(const Game& game){
		if (!game.gameStateStack.empty()) {
			return game.gameStateStack.back();
		}
		return GameState::None;
	}

	void PopGameState(Game& game) {
		if (!game.gameStateStack.empty()) {
			game.gameStateStack.pop_back();
		}
	}
}
