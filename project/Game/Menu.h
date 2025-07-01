#ifndef MENU_H
#define MENU_H

#include "Common.h"
#include "GameState.h"
#include "Game.h"

typedef struct MenuData
{
	sfTexture* texture;
	sfSprite* sprite;
}MenuData;

void LoadMenu(void);
void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _key);
void UpdateMenu(float _dt);
void DrawMenu(sfRenderWindow* _renderWindow);
void CleanupMenu(void);

#endif // !MENU_H
