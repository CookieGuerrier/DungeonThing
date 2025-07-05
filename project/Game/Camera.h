#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Math.h"

void LoadCamera(sfRenderWindow* _window);
void UpdateCamera(sfRenderWindow* _window, float _dt);
void DestroyCamera(void);

void MoveCamera(sfVector2f _pos, float _speed);
sfView* GetView(void);
sfView* GetHUDView(void);
void SetView(int _num, sfRenderWindow* _window);

#endif // !CAMERA_H