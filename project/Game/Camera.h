#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Math.h"

typedef struct Cursor
{
	sfTexture* texture;
	sfSprite* sprite;
}Cursor;

void LoadCamera(sfRenderWindow* _window);
void UpdateCamera(sfRenderWindow* _window, float _dt);
void DrawVeil(sfRenderWindow* _window);
void DestroyCamera(void);

void MoveCamera(sfVector2f _pos, float _speed);
void SetCamera(sfVector2f _pos);

sfView* GetView(void);
sfView* GetHUDView(void);
void SetView(int _num, sfRenderWindow* _window);

void SetDamageCamera(void);
void SetOpacityVeil(int _value, int _speed);

#endif // !CAMERA_H