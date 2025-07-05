#ifndef MAP_H
#define MAP_H

#include "Common.h"
#include "Camera.h"
#include "Player.h"
#include "Walls.h"
#include "Objects.h"
#include "Shop.h"
#include "MiniMap.h"

typedef enum MapType
{
	R,
	L,
	U,
	D,
	LR,
	LRU,
	LRD,
	LRUD,
	MAX
}MapType;

typedef enum ElementType
{
	BATTLE,
	BOSS,
	SHOP,
	START
}ElementType;

typedef enum BattleType
{
	ONE,
	TWO,
	MAX_LEVEL
}BattleType;

typedef struct MapPiece
{
	sfSprite* sprite;
	MapType type;
	ElementType element;
	sfVector2f pos;
	
	sfBool r;
	sfBool l;
	sfBool d;
	sfBool u;
}MapPiece;

void LoadMap(sfRenderWindow* _window);
void UpdateMap(float _dt, sfRenderWindow* _window);
void DrawMap(sfRenderWindow* _window, sfBool _debugMode);
void CleanupMap(void);

void AddMap(MapType _type, sfVector2f _pos, MapType _source, ElementType _element);
void DeleteMap(int _ID);
void CreateLevel(int _pathLength);

void CreateBattle(BattleType _type, sfFloatRect _hitbox);
void RemoveEnemyCurrent(void);
void CreateBattleBorder(sfVector2f _pos, MapType _type);

sfVector2f GetSpawnPoint(void);
void SetCameraMap(int _ID , sfRenderWindow* _window);
int GetCurrentMap(void);

#endif // !MAP_H
