#include "Walls.h"

Wall wall[500];
int wallCount;

void LoadWall(void)
{
	wallCount = 0;
}

void UpdateWall(float _dt, sfRenderWindow* _window)
{
}

void DrawWall(sfRenderWindow* _window, sfBool _debugMode)
{
	if (_debugMode)
	{
		for (int i = 0; i < wallCount; i++)
		{
			sfRenderWindow_drawRectangleShape(_window, wall[i].collider, NULL);
		}
	}
}

void CleanupWall(void)
{
	for (int i = 0; i < wallCount; i++)
	{
		DeleteWall(i);
	}
}

void AddWall(sfVector2f _pos, sfBool _rotate, sfVector2f _size, sfBool _isBreakable)
{
	if (wallCount < 500)
	{
		Wall wall2 = { 0 };
		wall2.collider = sfRectangleShape_create();
		wall2.isBreakable = _isBreakable;

		sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(wall2.collider);
		sfRectangleShape_setOrigin(wall2.collider, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });

		if (_rotate)
		{
			sfRectangleShape_setRotation(wall2.collider, 90);
		}

		sfRectangleShape_setSize(wall2.collider, _size);
		sfRectangleShape_setPosition(wall2.collider, _pos);
		sfRectangleShape_setFillColor(wall2.collider, (sfColor) { 255, 0, 0, 120 });

		wall[wallCount] = wall2;
		wallCount++;
	}
}

void DeleteWall(int _ID)
{
	sfRectangleShape_destroy(wall[_ID].collider);
	wall[_ID].collider = NULL;

	for (int i = _ID; i < wallCount - 1; i++)
	{
		Wall temp = wall[i];
		wall[i] = wall[i + 1];
		wall[i + 1] = temp;
	}

	wallCount--;
}

void MoveWall(int _ID, sfVector2f _pos)
{
	sfRectangleShape_setPosition(wall[_ID].collider, _pos);
}

sfBool BulletCollision(sfFloatRect _hitbox)
{
	for (int i = 0; i < wallCount; i++)
	{
		sfFloatRect hitbox = _hitbox;
		sfFloatRect collision = sfRectangleShape_getGlobalBounds(wall[i].collider);
		if (sfFloatRect_intersects(&collision, &hitbox, NULL))
		{
			if (wall[i].isBreakable)
			{
				DeleteWall(i);
			}
			return sfTrue;
		}
	}
	return sfFalse;
}


sfBool MoveCollision(sfFloatRect _direction)
{
	for (int i = 0; i < wallCount; i++)
	{
		sfFloatRect hitbox = _direction;
		sfFloatRect collision = sfRectangleShape_getGlobalBounds(wall[i].collider);
		if (sfFloatRect_intersects(&collision, &hitbox, NULL))
		{
			return sfTrue;
		}
	}
	return sfFalse;
}

int GetWallCount(void)
{
	return wallCount;
}
