#include "Game.h"

sfBool debugMode;

void LoadGame(sfRenderWindow* _window)
{
	LoadWall();
	LoadObject();
	LoadMoney();

	LoadEnemy();
	LoadShop();
	LoadMiniMap();
	LoadMap(_window);

	LoadPlayer();

	LoadBullet();

	LoadGameHUD();
}

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _key)
{
	switch (_key.code)
	{
	case sfKeyEscape:
		CleanupGame();
		LoadMenu();
		SetGameState(MENU);
		break;
	case sfKeySpace:
		CleanupGame();
		LoadGameOver();
		SetGameState(GAME_OVER);
		break;
	case sfKeyP:
		if (debugMode)
		{
			debugMode = sfFalse;
		}
		else
		{
			debugMode = sfTrue;
		}
		break;
	default:
		break;
	}
}

void UpdateGame(float _dt, sfRenderWindow* _window)
{
	UpdateWall(_dt, _window);
	UpdateObject(_dt, _window);
	UpdateMap(_dt, _window);
	UpdateMoney(_dt, _window);
	UpdateShop(_dt, _window);

	UpdateBullet(_dt, _window);
	UpdatePlayer(_dt, _window);
	UpdateMiniMap(_window, _dt);
	
	UpdateEnemy(_dt, _window);

	UpdateGameHUD(_dt);
}

void DrawGame(sfRenderWindow* _window)
{
	SetView(0, _window);
	DrawMap(_window, debugMode);
	DrawObject(_window, debugMode);
	DrawWall(_window, debugMode);
	DrawShop(_window, debugMode);

	DrawEnemy(_window, debugMode);
	DrawMoney(_window, debugMode);
	DrawBullet(_window, debugMode);
	DrawPlayer(_window, debugMode);

	SetView(1, _window);
	DrawGameHUD(_window, debugMode);
	
	SetView(2, _window);
	DrawMiniMap(_window, debugMode);
}

void CleanupGame(void)
{
	CleanupWall();
	CleanupObject();
	CleanupMap();
	CleanupMoney();
	CleanupShop();
		
	CleanupBullet();
	CleanupPlayer();
	CleanupEnemy();
	CleanupMiniMap();

	CleanupGameHUD();
}

