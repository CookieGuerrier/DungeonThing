#include "Camera.h"

sfView* view;
sfView* hudView;
sfView* miniView;
sfVector2f newPos;
sfVector2f move;
float speed;

void LoadCamera(sfRenderWindow* _window)
{
	view = sfView_create();
	hudView = sfView_create();
	miniView = sfView_create();
	sfView_setSize(view, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
	sfView_setSize(hudView, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
	sfView_setSize(miniView, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
	sfView_setViewport(miniView, (sfFloatRect) { 0.18f, -0.43f, 1, 1 });

	sfRenderWindow_setView(_window, view);
}

void UpdateCamera(sfRenderWindow* _window, float _dt)
{
	move = LerpVector2f(sfView_getCenter(view), newPos, speed * _dt);
	sfView_setCenter(view, move);
	sfRenderWindow_setView(_window, view);

	if (sfKeyboard_isKeyPressed(sfKeyM))
	{
		sfView_setSize(view, (sfVector2f) { SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10 });
	}
	if (sfKeyboard_isKeyPressed(sfKeyL))
	{
		sfView_setSize(view, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
	}
}

void DestroyCamera(void)
{
	sfView_destroy(view);
	view = NULL;
	sfView_destroy(hudView);
	hudView = NULL;
	sfView_destroy(miniView);
	miniView = NULL;
}

void MoveCamera(sfVector2f _pos, float _speed)
{
	newPos = _pos;
	speed = _speed;
}

sfView* GetView(void)
{
	return view;
}

sfView* GetHUDView(void)
{
	return hudView;
}

void SetView(int _num, sfRenderWindow* _window)
{
	switch (_num)
	{
	case 0:
		sfRenderWindow_setView(_window, view);
		break;
	case 1:
		sfRenderWindow_setView(_window, hudView);
		break;
	case 2:
		sfRenderWindow_setView(_window, miniView);
		break;
	default:
		break;
	}
}