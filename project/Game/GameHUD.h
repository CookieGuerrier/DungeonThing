#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "Common.h"
#include "Player.h"

typedef struct LifePoints
{
	sfSprite* sprite;
}LifePoints;

typedef struct GameHUD
{
	sfSprite* sprite;
	sfText* text;
}GameHUD;

void LoadGameHUD(void);
void UpdateGameHUD(float _dt);
void DrawGameHUD(sfRenderWindow* _window, sfBool _debug);
void CleanupGameHUD(void);

void AddLifePoint(void);
void DeleteLifePoint(void);

void UpdateText(sfText* const _text, int _value);
void UpdateGold(int _value);
#endif // !GAMEHUD_H
