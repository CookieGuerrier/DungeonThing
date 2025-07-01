#include "GameOver.h"

void LoadGameOver(void)
{
}

void KeyPressedGameOver(sfRenderWindow* _renderWindow, sfKeyEvent _key)
{
	switch (_key.code)
	{
	case sfKeyEscape:
		CleanupGameOver();
		LoadMenu();
		SetGameState(MENU);
		break;
	case sfKeySpace:
		CleanupGameOver();
		LoadGame(_renderWindow);
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void UpdateGameOver(float _dt)
{
}

void DrawGameOver(sfRenderWindow* _renderWindow)
{
}

void CleanupGameOver(void)
{
}
