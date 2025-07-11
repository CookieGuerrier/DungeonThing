#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Animator.h"
#include "Math.h"
#include "GameHUD.h"
#include "Map.h"
#include "Bullet.h"
#include "Camera.h"

typedef enum TexturePlayer
{
	IDLE,
	HAND,
	WALK
}TexturePlayer;

typedef struct Player
{
	sfSprite* sprite;
	sfSprite* spriteHand;
	sfSprite* spriteShadow;
	sfRectangleShape* collider;
	Anim** anims;

	int life;
	int gold;

	sfVector2f velocity;
	float speed;
	sfBool fireRate;
	float invFrame;
	float turnFrame;
	float hurtFrame;
}Player;

void LoadPlayer(void);
void LoadPlayerAnims(void);
void UpdatePlayer(float _dt, sfRenderWindow* _window);
void DrawPlayer(sfRenderWindow* _window, sfBool _debug);
void CleanupPlayer(void);

void PlayerMove(float _dt, sfRenderWindow* _window, sfKeyCode _key);
void PlayerShoot(float _dt);
void HandThing(sfVector2f _pos);
void PlayerTransition(int _num);

void LoseLife(int _life);
void GainLife(int _life);
void LoseGold(int _gold);
void GainGold(int _gold);

int GetHP(void);
int GetMoney(void);
sfVector2f GetPlayerPos(void);
sfFloatRect GetPlayerHitbox(void);
sfVector2f GetMousePos(void);
#endif // !PLAYER_H
