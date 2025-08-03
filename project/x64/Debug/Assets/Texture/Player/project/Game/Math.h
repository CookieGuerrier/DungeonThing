#ifndef MATH_H
#define MATH_H

#include "Common.h"
#include <math.h>
#include <corecrt_math_defines.h>

float LookToDirection(sfVector2f _target, sfVector2f _origin);
float GetDistanceVector2f(sfVector2f _a, sfVector2f _b);
sfVector2f LerpVector2f(sfVector2f _a, sfVector2f _b, float _lenght);

#endif // !MATH_H