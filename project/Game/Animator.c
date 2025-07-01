#include "Animator.h"

void TestEvents(Anim* _anim);

Anim* CreateAnim(sfTexture* _texture, sfIntRect _first, int _frameMax, float _rate, sfSprite* _sprite, sfVector2f _origin, sfBool _loop)
{
	Anim* tempAnim = malloc(sizeof(Anim));
	if (tempAnim != NULL)
	{
		tempAnim->eventNum = 0;
		tempAnim->firstState = _first;
		tempAnim->texture = _texture;
		tempAnim->frameMax = _frameMax;
		tempAnim->frameNum = 0;
		tempAnim->time = 0;
		tempAnim->rate = _rate;
		tempAnim->sprite = _sprite;
		tempAnim->origin = _origin;
		tempAnim->looping = _loop;
		tempAnim->events = NULL;
		refreshFrame(tempAnim);
		return tempAnim;
	}
	else
	{
		free(tempAnim);
		return NULL;
	}
}

void UpdateAnim(float _dt, Anim* _anim)
{
	if (_anim->looping || !(_anim->frameMax == _anim->frameNum))
	{
		_anim->time += _dt;
		while (_anim->time > _anim->rate)
		{
			_anim->time -= _anim->rate;
			_anim->frameNum++;
			refreshFrame(_anim);
		}
	}
}

void TestEvents(Anim* _anim)
{
	if (_anim->events != NULL)
	{
		for (int i = 0; i < _anim->eventNum; i++)
		{
			if (_anim->events[i].frameNum == _anim->frameNum)
			{
				_anim->events[i].function();
			}
		}
	}
}

sfBool IsFinishedAnim(Anim* _anim)
{
	return (_anim->frameMax <= _anim->frameNum);
}

sfBool IsStarting(Anim* _anim)
{
	return (0 == _anim->frameNum);
}

void refreshFrame(Anim* _anim)
{
	if (_anim != NULL)
	{
		if (_anim->frameMax <= _anim->frameNum)
		{
			if (_anim->looping)
			{
				_anim->frameNum = 0;
			}
			else
			{
				return;
			}
		}
		TestEvents(_anim);
		sfIntRect tempFrame = _anim->firstState;
		tempFrame.left += tempFrame.width * _anim->frameNum;
		sfSprite_setTexture(_anim->sprite, _anim->texture, sfFalse);
		sfSprite_setTextureRect(_anim->sprite, tempFrame);
		sfSprite_setOrigin(_anim->sprite, _anim->origin);
	}
	else
	{
		return;
	}
}

void ResetAnim(Anim* _anim)
{
	_anim->frameNum = 0;
	_anim->time = 0;
	refreshFrame(_anim);
}

void DestroyAnim(Anim* _anim)
{
	if (_anim->events != NULL)
	{
		free((_anim)->events);
		(_anim)->events = NULL;
	}
	free((_anim));
	_anim = NULL;
}
