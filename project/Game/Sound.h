#ifndef SOUND_H
#define SOUND_H

#include "Common.h"

typedef enum SoundType
{
	S_STEP,
	S_DASH,
	S_SHOOT,
	S_HURT,
	S_BULLET,
	S_MONEY,
	S_SHOP,
	S_SELECTION,

	S_ENEMYHURT,
	S_ENEMYDEATH,

	S_NAKROMSUMMON,
	S_MAX
}SoundType;

typedef struct Sound
{
	sfSound* sound;
	SoundType type;
}Sound;

void LoadSound(void);
void CleanupSound(void);

void AddSound(SoundType _type);
void DeleteSound(int _ID);
void PlaySound(int _ID);

int GetSoundCount(void);
#endif // !SOUND_H