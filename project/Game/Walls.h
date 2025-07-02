#ifndef WALLS_H
#define WALLS_H

#include "Common.h"
#include "Camera.h"
#include "Player.h"

typedef struct Wall 
{
	sfRectangleShape* collider;
	sfBool isBreakable;
}Wall;

void LoadWall(void);
void UpdateWall(float _dt, sfRenderWindow* _window);
void DrawWall(sfRenderWindow* _window, sfBool _debugMode);
void CleanupWall(void);

void AddWall(sfVector2f _pos, sfBool _rotate, sfVector2f _size, sfBool _isBreakable);
void DeleteWall(int _ID);
void MoveWall(int _ID, sfVector2f _pos);

sfBool BulletCollision(sfFloatRect _hitbox);
sfBool MoveCollision(sfFloatRect _direction);
sfBool ObjectCollision(sfVector2f _direction);

int GetWallCount(void);

#endif // !WALLS_H