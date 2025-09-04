#ifndef MENUHUD_H
#define MENUHUD_H

#include "Common.h"
#include "Camera.h"
#include "Player.h"

typedef struct Buttons
{
	sfText* text;
}Buttons;

typedef struct Leaf
{
	sfRectangleShape* sprite;
	sfBool turningLeft;
	float x;
	float y;
	sfBool slow;
}Leaf;

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
sfBool IsMouseOnButtonMenuHUD(void);

void AddLeaf(sfVector2f _position);
void DeleteLeaf(int _ID);
#endif // !MENUHUD_H
