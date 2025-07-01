#include "Walls.h"

Wall* wall = NULL;
int wallCount;

void LoadWall(void)
{
	wallCount = 0;
	wall = calloc(1, sizeof(LifePoints));
	if (!wall)
	{
		fprintf(stderr, "Erreur d'allocation memoire pour wallList.\n");
		return;
	}
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

void AddWall(sfVector2f _pos, sfBool _rotate, sfVector2f _size)
{
	Wall* temp = realloc(wall, (size_t)(wallCount + 1) * sizeof(Wall));
	if (!temp) {
		fprintf(stderr, "WallList reallocation failed!\n");
		return;
	}
	wall = temp;

	Wall wall2 = { 0 };
	wall2.collider = sfRectangleShape_create();

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

void DeleteWall(int _ID)
{
	Wall* temp = realloc(wall, (size_t)(wallCount - 1) * sizeof(Wall));
	if (!temp) {
		fprintf(stderr, "Enemy list reallocation failed!\n");
		return;
	}

	wall = temp;
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
