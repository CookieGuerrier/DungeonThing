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
		wallCount++;
	}
}

void AddWall(sfVector2f _pos, sfBool _rotate, sfVector2f _size, sfBool _bulletThrough, int _object)
{
	if (wallCount < 500)
	{
		Wall wall2 = { 0 };
		wall2.collider = sfRectangleShape_create();
		wall2.bulletThrough = _bulletThrough;

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

void ClearWall(void)
{
	for (int i = 4; i < wallCount; i++)
	{
		DeleteWall(i);
		wallCount++;
	}
	wallCount = 4;
}

void MoveWall(int _ID, sfVector2f _pos)
{
	sfRectangleShape_setPosition(wall[_ID].collider, _pos);
}

sfBool BulletCollision(sfFloatRect _hitbox, sfVector2f* _vel)
{
	for (int i = 0; i < wallCount; i++)
	{
		sfFloatRect hitbox = _hitbox;
		sfFloatRect collision = sfRectangleShape_getGlobalBounds(wall[i].collider);
		if (sfFloatRect_intersects(&collision, &hitbox, NULL))
		{
			if (!wall[i].bulletThrough)
			{
				sfVector2f posBul = (sfVector2f) { hitbox.left + hitbox.width / 2,  hitbox.top + hitbox.height / 2 };
				sfVector2f posHit = (sfVector2f) { collision.left + collision.width / 2, collision.top + collision.height / 2 };

				if (posBul.y < posHit.y - collision.height / 2 + 20 || posBul.y > posHit.y + collision.height / 2 - 20)
				_vel->y *= -1;
				else
				_vel->x *= -1;

				return sfTrue;
			}
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

sfBool ObjectCollision(sfVector2f _direction)
{
	for (int i = 0; i < wallCount; i++)
	{
		sfVector2f pos = _direction;
		sfFloatRect collision = sfRectangleShape_getGlobalBounds(wall[i].collider);
		if (sfFloatRect_contains(&collision, pos.x, pos.y))
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
