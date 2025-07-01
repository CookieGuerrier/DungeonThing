#ifndef GAME_STATE_H
#define GAME_STATE_H

#pragma region Enum
typedef enum GameState
{
	MENU,
	GAME,
	GAME_OVER
}GameState;
#pragma endregion

void SetGameState(GameState _gameState);
GameState GetGameState(void);

#endif // !GAME_STATE_H
