#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "Common.h"
#include "Player.h"

typedef struct LifePoints
{
	sfSprite* sprite;
}LifePoints;

typedef struct ArtifactSlot
{
	sfSprite* sprite;
}ArtifactSlot;

typedef struct GameHUD
{
	sfSprite* sprite;
	sfText* text;
}GameHUD;

void LoadGameHUD(sfFont* font);
void UpdateGameHUD(float _dt);
void DrawGameHUD(sfRenderWindow* _window, sfBool _debug);
void CleanupGameHUD(sfFont* font);

void AddLife(void);
void DeleteLife(void);

void UpdateText(sfText* const _text, int _value);
void UpdateGold(int _value);
void UpdateSlot(int _ID, sfTexture* _texture);
#endif // !GAMEHUD_H
