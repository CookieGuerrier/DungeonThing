#include "GameState.h"

GameState gameState;

void SetGameState(GameState _gameState)
{
    gameState = _gameState;
}

GameState GetGameState(void)
{
    return gameState;
}
