#ifndef SHOP_H
#define SHOP_H

#include "Common.h"
#include "Player.h"
#include "GameHUD.h"

typedef enum ShopType
{
	POTION,
	BIG_POTION
}ShopType;


typedef struct ItemShop
{
	sfSprite* sprite;
	sfBool isActive;
	ShopType type;
	sfText* text;
}ItemShop;

void LoadShop(void);
void UpdateShop(float _dt, sfRenderWindow* _window);
void DrawShop(sfRenderWindow* _window, sfBool _debug);
void CleanupShop(void);

void AddItem(int _num, ShopType _type, sfVector2f _position);

#endif // !SHOP_H