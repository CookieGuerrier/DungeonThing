#ifndef MENUHUD_H
#define MENUHUD_H

#include "Common.h"
#include "Camera.h"
#include "Player.h"

typedef struct Buttons
{
	sfText* text;
}Buttons;

typedef struct Selection
{
	sfSprite* sprite;
}Selection;

void LoadMenuHUD(void);
void UpdateMenuHUD(float _dt, sfRenderWindow* _window);
void DrawMenuHUD(sfRenderWindow* _window);
void CleanupMenuHUD(void);

void SetSelection(int _sel);
int GetSelection(void);
#endif // !MENUHUD_H
