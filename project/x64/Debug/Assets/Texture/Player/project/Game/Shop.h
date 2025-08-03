#ifndef SHOP_H
#define SHOP_H

#include "Common.h"
#include "Player.h"
#include "GameHUD.h"

typedef enum ShopType
{
	//Artifacts
	SAW,
	CONTRACT,
	BOOMERANG,
	BLUE_GEL,
	CHRONO,
	STAMP,

	//Heal 
	POTION,
	BIG_POTION
}ShopType;


typedef struct ItemShop
{
	sfSprite* sprite;
	sfBool isActive;
	ShopType type;
	sfText* text;
	sfBool free;
}ItemShop;

void LoadShop(void);
void UpdateShop(float _dt, sfRenderWindow* _window);
void DrawShop(sfRenderWindow* _window, sfBool _debug);
void CleanupShop(void);

void AddItem(ShopType _type, sfVector2f _position, sfBool _free);
void DeleteItem(int _ID);
void ItemDescription(void);

#endif // !SHOP_H