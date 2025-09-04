#include "Sound.h"

Sound sound[30];
sfSoundBuffer* buffer[S_MAX];
int soundCount;

void LoadSound(void)
{
	soundCount = 0;
	buffer[S_STEP] = sfSoundBuffer_createFromFile("Assets/Sounds/Player/step.ogg");
	buffer[S_SELECTION] = sfSoundBuffer_createFromFile("Assets/Sounds/menuSelect.ogg");
	buffer[S_SHOOT] = sfSoundBuffer_createFromFile("Assets/Sounds/Player/shoot.ogg");
	buffer[S_HURT] = sfSoundBuffer_createFromFile("Assets/Sounds/Player/hurt.ogg");
	buffer[S_BULLET] = sfSoundBuffer_createFromFile("Assets/Sounds/Player/bullet.ogg");
	buffer[S_SHOP] = sfSoundBuffer_createFromFile("Assets/Sounds/Player/shop.ogg");
	buffer[S_MONEY] = sfSoundBuffer_createFromFile("Assets/Sounds/Player/money.ogg");
	buffer[S_ENEMYDEATH] = sfSoundBuffer_createFromFile("Assets/Sounds/Enemy/death.ogg");
	buffer[S_ENEMYHURT] = sfSoundBuffer_createFromFile("Assets/Sounds/Enemy/hurt.ogg");
	buffer[S_NAKROMSUMMON] = sfSoundBuffer_createFromFile("Assets/Sounds/Enemy/nakromSummon.ogg");

	AddSound(S_SELECTION);
	AddSound(S_SELECTION);
}

void CleanupSound(void)
{
	for (int i = 0; i < S_MAX; i++)
	{
		sfSoundBuffer_destroy(buffer[i]);
		buffer[i] = NULL;
	}
	for (int i = 0; i < soundCount; i++)
	{
		DeleteSound(i);
		soundCount++;
	}
}

void AddSound(SoundType _type)
{
	Sound temp = { 0 };
	temp.sound = sfSound_create();
	temp.type = _type;
	sfSound_setBuffer(temp.sound, buffer[_type]);
	switch (_type)
	{
	case S_STEP:
	case S_SHOOT:
		sfSound_setVolume(temp.sound, 30);
		break;
	case S_SHOP:
	case S_NAKROMSUMMON:
	case S_BULLET:
		sfSound_setVolume(temp.sound, 50);
		break;
	case S_SELECTION:
		if (soundCount == 1)
			sfSound_setPitch(temp.sound, 0.84f);
		sfSound_setVolume(temp.sound, 60);
		break;
	case S_ENEMYDEATH:
	case S_ENEMYHURT:
		sfSound_setVolume(temp.sound, 70);
		break;
	default:
		break;
	}
	sound[soundCount] = temp;
	soundCount++;
} 

void DeleteSound(int _ID)
{
	sfSound_destroy(sound[_ID].sound);
	sound[_ID].sound = NULL;

	for (int i = _ID; i < soundCount - 1; i++)
	{
		Sound temp = sound[i];
		sound[i] = sound[i + 1];
		sound[i + 1] = temp;
	}

	soundCount--;
}

void PlaySound(int _ID)
{
	if (sound[_ID].type == S_STEP)
	{
		sfSound_setPitch(sound[_ID].sound, 1 + (float) (rand() % 3 - 1) / 10);
	}
	if (sound[_ID].type == S_SHOOT)
	{
		sfSound_setPitch(sound[_ID].sound, 1.4f + (float) (rand() % 3 - 1) / 10);
	}
	sfSound_play(sound[_ID].sound);
}

int GetSoundCount(void)
{
	return soundCount;
}