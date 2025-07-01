#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include "Player.h"

typedef enum TypeEnemy
{
	REA_BASE,
	REA_SHOTGUN
}TypeEnemy;

typedef enum TextureEnemy
{
	IDLE_E,
	WALK_E,
	DEATH_E
}TextureEnemy;

typedef struct Enemy
{
	sfSprite* sprite;
	sfSprite* spriteShadow;
	sfRectangleShape* collider;
	TypeEnemy type;
	int id;

	Anim** anims;

	int life;
	sfBool isDead;
	sfVector2f velocity;
	float speed;
	sfBool isShooting;

	float fireRate;
	int fireThing;
	float hurtFrame;
}Enemy;


void LoadEnemy(void);
void LoadEnemyAnimation(Enemy* _enemy);
void UpdateEnemy(float _dt, sfRenderWindow* _window);
void DrawEnemy(sfRenderWindow* _window, sfBool _debug);
void CleanupEnemy(void);

void AddEnemy(TypeEnemy _type, sfVector2f _pos, int _idMap);
void DeleteEnemy(int _ID);

void EnemyMove(int _ID, float _dt);
void EnemyShoot(int _ID, float _dt);
void EnemyHandThing(int _ID, sfVector2f _pos);
void EnemyHurt(int _ID);

int GetEnemyCount(void);
sfFloatRect GetEnemyHitBox(int _ID);
sfBool IsEnemyAlive(int _ID);
int GetEnemyMap(int _ID);
#endif // !ENEMY_H