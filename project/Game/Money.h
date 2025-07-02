#ifndef MONEY_H
#define MONEY_H

#include "Common.h"
#include "Player.h"

typedef struct Nugget
{
	sfSprite* sprite;
	sfVector2f velocity;
	int speed;
}Nugget;

void LoadMoney(void);
void UpdateMoney(float _dt, sfRenderWindow* _window);
void DrawMoney(sfRenderWindow* _window, sfBool _debug);
void CleanupMoney(void);

void AddNugget(sfFloatRect _hitbox);
void DeleteNugget(int _ID);

#endif // !MONEY_H