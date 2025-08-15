#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Animator.h"
#include "Enemy.h"

typedef enum Boss
{
	NAKROM
}Boss;

typedef enum TextureBoss
{
	IDLE_BOSS,
	ATTACK_BOSS,
	ATTACK2_BOSS,
	DEATH_BOSS
}TextureBoss;

typedef struct Nakrom
{
	int hp;
	int color;
	sfSprite* sprite;
	sfVector2f velocity;

	sfRectangleShape* collider;
	Anim** anims;

	TextureBoss state;
	float stateTimer;
	float fireRate;
	float fireThing;
	int fireState;
	sfBool spawn;
	int spawnMax;

	float hurtFrame;
}Nakrom;


void LoadBoss(void);
void UpdateBoss(float _dt, sfRenderWindow* _window);
void DrawBoss(sfRenderWindow* _window, sfBool _debug);
void CleanupBoss(void);

void AddBoss(Boss _boss, sfVector2f _position, int _id);
void DeleteBoss(void);

void HurtBoss(int _dmg);
void RopeBoss(void);

sfFloatRect GetBossHitbox(void);
sfBool GetBossDead(void);
int GetBossHP(void);
int GetBossActive(void);
float GetBossHurtFrame(void);
int GetBossID(void);
#endif // !BOSS_H