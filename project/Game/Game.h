#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"
#include "Menu.h"
#include "GameOver.h"

#include "Walls.h"
#include "Objects.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Camera.h"
#include "Shop.h"
#include "MiniMap.h"
#include "OverlayMap.h"

typedef struct GameData
{
	sfTexture* texture;
	sfSprite* sprite;
}GameData;

void LoadGame(sfRenderWindow* _window);
void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _key);
void MousePressedGame(sfRenderWindow* _renderWindow, sfMouseButtonEvent _mouse);
void UpdateGame(float _dt, sfRenderWindow* _window);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

void LoadPauseMenu(void);
void UpdatePauseMenu(float _dt, sfRenderWindow* _window);
void DrawPauseMenu(sfRenderWindow* _window);
void CleanupPauseMenu(void);

void UpdateGameOverMenu(float _dt, sfRenderWindow* _window);
void DrawGameOverMenu(sfRenderWindow* _window);
void SetSelectionGameOver(int _sel);
sfBool IsMouseOnButtonGame(void);

#endif // !GAME_H
