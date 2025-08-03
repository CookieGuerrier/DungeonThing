#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Common.h"

typedef struct AnimEvent
{
	int frameNum;
	void (*function)();
}AnimEvent;

typedef struct Anim
{
	float time;
	float rate;
	sfTexture* texture;
	int frameNum;
	int frameMax;
	sfIntRect firstState;
	AnimEvent* events;
	int eventNum;
	sfSprite* sprite;
	sfVector2f origin;
	sfBool looping;
	sfVector2f aimOffset;
}Anim;

Anim* CreateAnim(sfTexture* _texture, sfIntRect _first, int _frameMax, float _rate, sfSprite* _sprite, sfVector2f _origin, sfBool _loop);
void UpdateAnim(float _dt, Anim* _anim);

void refreshFrame(Anim* _anim);
sfBool IsFinishedAnim(Anim* _anim);
sfBool IsStarting(Anim* _anim);
void ResetAnim(Anim* _anim);
void DestroyAnim(Anim* _anim);


#endif
