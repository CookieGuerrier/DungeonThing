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
#include "Camera.h"
#include "Shop.h"
#include "MiniMap.h"

typedef struct GameData
{
	sfTexture* texture;
	sfSprite* sprite;
}GameData;

void LoadGame(sfRenderWindow* _window);
void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _key);
void UpdateGame(float _dt, sfRenderWindow* _window);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

#endif // !GAME_H
