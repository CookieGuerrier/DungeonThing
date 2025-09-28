#include "Camera.h"

sfView* view;
sfView* hudView;
sfView* miniView;
sfVector2f newPos;
sfVector2f move;
float speed;
sfBool damage;

int opacity;
int newOpacity;
int opacitySpeed;

sfRectangleShape* veil;
Cursor cursor;

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

	veil = sfRectangleShape_create();
	sfRectangleShape_setSize(veil, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(veil);
	sfRectangleShape_setOrigin(veil, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfRectangleShape_setPosition(veil, (sfVector2f) { 500, 500 });
	sfRectangleShape_setFillColor(veil, (sfColor) { 0, 0, 0, 0 });

	sfRenderWindow_setVerticalSyncEnabled(_window, sfTrue);

	cursor.sprite = sfSprite_create();
	cursor.texture = sfTexture_createFromFile("Assets/Texture/Player/cursor.png", NULL);
	sfSprite_setTexture(cursor.sprite, cursor.texture, sfTrue);
	hitbox = sfSprite_getGlobalBounds(cursor.sprite);
	sfSprite_setOrigin(cursor.sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });

	sfRenderWindow_setMouseCursorVisible(_window, sfFalse);
}

void UpdateCamera(sfRenderWindow* _window, float _dt)
{
	move = LerpVector2f(sfView_getCenter(view), newPos, speed * _dt);
	sfView_setCenter(view, move);
	sfRenderWindow_setView(_window, view);

	if (damage)
	{
		if (sfView_getRotation(view) != 1.f)
		{
			sfView_rotate(view, -0.5f);
		}
		else
		{
			damage = sfFalse;
			sfView_setRotation(view, 0);
		}
	}

	if (opacity < newOpacity)
	{
		newOpacity -= opacitySpeed;
		sfRectangleShape_setFillColor(veil, (sfColor) { 0, 0, 0, newOpacity });
	}
	else if (opacity > newOpacity)
	{
		newOpacity += opacitySpeed;
		sfRectangleShape_setFillColor(veil, (sfColor) { 0, 0, 0, newOpacity });
	}
	// :(
	if (newOpacity < 10)
	{
		sfRectangleShape_setFillColor(veil, (sfColor) { 0, 0, 0, 0 });
	}

	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	sfVector2f posMouse = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetHUDView());
	sfSprite_setPosition(cursor.sprite, posMouse);
}

void DrawVeil(sfRenderWindow* _window)
{
	sfRenderWindow_drawRectangleShape(_window, veil, NULL);
}

void DrawCursor(sfRenderWindow* _window)
{
	sfRenderWindow_drawSprite(_window, cursor.sprite, NULL);
}

void DestroyCamera(void)
{
	sfView_destroy(view);
	view = NULL;
	sfView_destroy(hudView);
	hudView = NULL;
	sfView_destroy(miniView);
	miniView = NULL;
	sfRectangleShape_destroy(veil);
	veil = NULL;

	sfTexture_destroy(cursor.texture);
	cursor.texture = NULL;
	sfSprite_destroy(cursor.sprite);
	cursor.sprite = NULL;
}

void MoveCamera(sfVector2f _pos, float _speed)
{
	newPos = _pos;
	speed = _speed;
}

void SetCamera(sfVector2f _pos)
{
	sfView_setCenter(view, _pos);
}

sfView* GetView(void)
{
	return view;
}

sfView* GetHUDView(void)
{
	return hudView;
}

int GetOpacity(void)
{
	return newOpacity;
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

void SetDamageCamera(void)
{
	sfView_setRotation(view, 5);
	damage = sfTrue;
}

void SetOpacityVeil(int _value, int _speed)
{
	opacity = _value;
	opacitySpeed = _speed;
}