#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "Common.h"
#include "Player.h"

typedef struct LifePoints
{
	sfSprite* sprite;
}LifePoints;

void LoadGameHUD(void);
void UpdateGameHUD(float _dt);
void DrawGameHUD(sfRenderWindow* _window, sfBool _debug);
void CleanupGameHUD(void);

void AddLifePoint(void);
void DeleteLifePoint(void);
#endif // !GAMEHUD_H
