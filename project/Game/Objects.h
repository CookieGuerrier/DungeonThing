#ifndef OBJECT_H
#define OBJECT_H

#include "Common.h"
#include "Walls.h"
#include "Animator.h"

typedef enum ObjectType
{
	WALL_BLOCKUD,
	WALL_BLOCKRL,
	WALL_BATTLEUD,
	WALL_BATTLERL,
	POT,
	STATUE,
	BIG_HOLE,
	LONG_HOLE,
	ROCK,
	TORCH,
	COFFIN,
	EXIT_HOLE
}ObjectType;

typedef struct Object
{
	sfSprite* sprite;
	sfSprite* shadow;
	int wallID;
	ObjectType type;
	Anim** anims;
}Object;

void LoadObject(void);
void UpdateObject(float _dt, sfRenderWindow* _window);
void DrawObject(sfRenderWindow* _window, sfBool _debug);
void CleanupObject(void);

void AddObject(sfVector2f _pos, float _rot, ObjectType _type);
void DeleteObject(int _ID);
void ClearObject(void);

void MoveObject(int _ID, sfVector2f _pos);
void SetExitActive(void);

#endif // !OBJECT_H