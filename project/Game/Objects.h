#ifndef OBJECT_H
#define OBJECT_H

#include "Common.h"
#include "Walls.h"

typedef enum ObjectType
{
	WALL_BLOCKUD,
	WALL_BLOCKRL,
	WALL_BATTLEUD,
	WALL_BATTLERL,
	POT
}ObjectType;

typedef struct Object
{
	sfSprite* sprite;
	sfSprite* shadow;
	int wallID;
	ObjectType type;
}Object;

void LoadObject(void);
void UpdateObject(float _dt, sfRenderWindow* _window);
void DrawObject(sfRenderWindow* _window, sfBool _debug);
void CleanupObject(void);

void AddObject(sfVector2f _pos, float _rot, ObjectType _type);
void DeleteObject(int _ID);

void MoveObject(int _ID, sfVector2f _pos);

#endif // !OBJECT_H