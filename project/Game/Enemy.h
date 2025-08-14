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
	BAT,
	BIG_CRAB,
	TORMENTED_SOUL,
	TINY_CRAB,
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
	sfBool isShooting;
	float speed;
	int color;

	float fireRate;
	int fireThing;
	float hurtFrame;
	float stateTimer;
	float misc;
	int state;
	float playerRot;
	float nerf;
}Enemy;

typedef struct Marker
{
	sfText* text;
	int alpha;
	int dmg;
}Marker;

void LoadEnemy(void);
void LoadEnemyAnimation(Enemy* _enemy);
void UpdateEnemy(float _dt, sfRenderWindow* _window);
void DrawEnemy(sfRenderWindow* _window, sfBool _debug);
void DrawMarker(sfRenderWindow* _window);
void CleanupEnemy(void);

void AddEnemy(TypeEnemy _type, sfVector2f _pos, int _idMap);
void DeleteEnemy(int _ID);
void ClearEnemy(void);

void AddMarker(sfVector2f _pos, int _dmg);
void DeleteMarker(int _ID);

void EnemyMove(int _ID, float _dt);
void EnemyShoot(int _ID, float _dt);
void EnemyHurt(int _ID, int _dmg);
void EnemyDeath(int _ID, float _dt);
sfVector2f RopeEnemy(int _ID);

int GetEnemyCount(void);
sfFloatRect GetEnemyHitBox(int _ID);
sfBool IsEnemyAlive(int _ID);
int GetEnemyMap(int _ID);
float GetEnemyHurtFrame(int _ID);
#endif // !ENEMY_H