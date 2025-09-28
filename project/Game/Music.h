#ifndef MUSIC_H
#define MUSIC_H

#include "Common.h"

typedef struct Music
{
	sfMusic* music;
}Music;

void LoadMusic(void);
void UpdateMusic(float _dt);
void DestroyMusic(void);

void PlayMusic(int _mus);
void StopMusic(void);
sfSoundStatus EndMusic(void);
#endif // !MUSIC_H