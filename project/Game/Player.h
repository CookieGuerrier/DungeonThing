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

void GainLife(int _life);
void LoseLife(int _life);
void PlayerTransition(sfRenderWindow* _window);

int GetHP(void);
sfVector2f GetPlayerPos(void);
sfFloatRect GetPlayerHitbox(void);
sfVector2f GetMousePos(void);
#endif // !PLAYER_H
