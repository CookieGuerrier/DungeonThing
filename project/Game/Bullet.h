#ifndef BULLET_H
#define BULLET_H

#include "Common.h"
#include "Player.h"
#include "Enemy.h"

typedef struct Bullet
{
	sfSprite* sprite;
	sfVector2f velocity;
	sfBool friendlyFire;
	sfBool death;
	float rotation;
	int bounce;
	int speed;
	int dmg;
	Anim** anims;
}Bullet;

void LoadBullet(void);
void UpdateBullet(float _dt, sfRenderWindow* _window);
void DrawBullet(sfRenderWindow* _window, sfBool _debug);
void CleanupBullet(void);

void AddBullet(sfVector2f _pos, float _rot, int _speed, sfBool _friendlyFire, sfBool _bigBullet);
void DeleteBullet(int _ID);

#endif // !BULLET_H