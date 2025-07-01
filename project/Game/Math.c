#include "Math.h"

float LookToDirection(sfVector2f _target, sfVector2f _origin)
{
	sfVector2f diff = { _target.x - _origin.x ,  _target.y - _origin.y };

	return atan2f(diff.y, diff.x) * (float)(180 / M_PI);
}

float GetDistanceVector2f(sfVector2f _a, sfVector2f _b)
{
	return sqrtf((_b.x - _a.x) * (_b.x - _a.x) + (_b.y - _a.y) * (_b.y - _a.y));
}

sfVector2f LerpVector2f(sfVector2f _a, sfVector2f _b, float _lenght)
{
	//Stolen from Guillaume
	sfVector2f val = { 0 };
	val.x = (1 - _lenght) * _a.x + _lenght * _b.x;
	val.y = (1 - _lenght) * _a.y + _lenght * _b.y;
	return val;
}