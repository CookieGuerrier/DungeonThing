#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include "Player.h"
#include "Money.h"

typedef enum TypeEnemy
{
	REA_BASE,
	REA_SHOTGUN,
	REA_CLOTH,
	SLIME,
	BIG_CRAB,
	TORMENTED_SOUL,
	TINY_CRAB
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
	int color;

	float fireRate;
	int fireThing;
	float hurtFrame;
	float stateTimer;
	float misc;
	int state;
	float playerRot;
}Enemy;

typedef struct Marker
{
	sfText* text;
	int alpha;
}Marker;

void LoadEnemy(void);
void LoadEnemyAnimation(Enemy* _enemy);
void UpdateEnemy(float _dt, sfRenderWindow* _window);
void DrawEnemy(sfRenderWindow* _window, sfBool _debug);
void CleanupEnemy(void);

void AddEnemy(TypeEnemy _type, sfVector2f _pos, int _idMap);
void DeleteEnemy(int _ID);

void AddMarker(sfVector2f _pos, int _dmg);
void DeleteMarker(int _ID);

void EnemyMove(int _ID, float _dt);
void EnemyShoot(int _ID, float _dt);
void EnemyHurt(int _ID, int _dmg);

int GetEnemyCount(void);
sfFloatRect GetEnemyHitBox(int _ID);
sfBool IsEnemyAlive(int _ID);
int GetEnemyMap(int _ID);
sfVector2f GetClosestEnemy(sfVector2f _pos);
#endif // !ENEMY_H