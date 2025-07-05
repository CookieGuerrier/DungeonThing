#ifndef MINIMAP_H
#define MINIMAP_H

#include "Common.h"
#include "Map.h"

typedef struct MiniMap
{
	sfSprite* sprite;
	sfBool disco;
}MiniMap;

void LoadMiniMap(void);
void UpdateMiniMap(sfRenderWindow* _window, float _dt);
void DrawMiniMap(sfRenderWindow* _window, sfBool _debugMode);
void CleanupMiniMap(void);

void AddMiniMap(sfVector2f _position, int _type);
void DeleteMiniMap(int _ID);

void PositionMini(int _ID);

#endif // !MINIMAP_H