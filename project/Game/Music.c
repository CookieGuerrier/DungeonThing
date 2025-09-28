#include "Music.h"
#include "Camera.h"

Music music[3];
int currentVolume;
int currentMusic;

void LoadMusic(void)
{
	currentVolume = 60;
	music[0].music = sfMusic_createFromFile("Assets/Music/mainMenu.ogg");
	music[1].music = sfMusic_createFromFile("Assets/Music/battleTheme.ogg");
	music[2].music = sfMusic_createFromFile("Assets/Music/victory.ogg");
	sfMusic_setVolume(music[1].music, 10);
	sfMusic_setLoop(music[0].music, sfTrue);
	sfMusic_setLoop(music[1].music, sfTrue);
}

void UpdateMusic(float _dt)
{
	float per = (float) GetOpacity() / 255 * 100;
	sfMusic_setVolume(music[currentMusic].music, currentVolume - per);
}

void DestroyMusic(void)
{
	sfMusic_destroy(music[0].music);
	music[0].music = NULL;
	sfMusic_destroy(music[1].music);
	music[1].music = NULL;
	sfMusic_destroy(music[2].music);
	music[2].music = NULL;
}

void PlayMusic(int _mus)
{
	sfMusic_stop(music[currentMusic].music);
	currentMusic = _mus;
	sfMusic_play(music[_mus].music);
}

void StopMusic()
{
	sfMusic_stop(music[currentMusic].music);
}

sfSoundStatus EndMusic(void)
{
	return	sfMusic_getStatus(music[2].music);;
}
