#include "Objects.h"

Object object[300];
sfTexture* textureObject[5];
sfTexture* shadowTexture;
int objectCount;

int wall1, wall2, wall3, wall4;

void LoadObject(void)
{
	objectCount = 0;
	textureObject[WALL_BLOCKUD] = sfTexture_createFromFile("Assets/Texture/Objects/wallUD.png", NULL);
	textureObject[WALL_BLOCKRL] = sfTexture_createFromFile("Assets/Texture/Objects/wallRL.png", NULL);
	textureObject[WALL_BATTLEUD] = sfTexture_createFromFile("Assets/Texture/Objects/blockUD.png", NULL);
	textureObject[WALL_BATTLERL] = sfTexture_createFromFile("Assets/Texture/Objects/blockRL.png", NULL);

	textureObject[POT] = sfTexture_createFromFile("Assets/Texture/Objects/pot.png", NULL);
	shadowTexture = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);

	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLERL);
	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLERL);
	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLEUD);
	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLEUD);
}

void UpdateObject(float _dt, sfRenderWindow* _window)
{
	for (int i = 0; i < objectCount; i++)
	{
		if (object[i].type == WALL_BATTLERL || object[i].type == WALL_BATTLEUD)
		{
			sfVector2f pos = sfSprite_getPosition(object[i].sprite);
			sfFloatRect hitbox = sfSprite_getGlobalBounds(object[i].sprite);
			MoveWall(i, (sfVector2f) { pos.x - hitbox.width / 2, pos.y - hitbox.height / 2 });
		}
	}
}

void DrawObject(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < objectCount; i++)
	{
		if (object[i].type == POT)
		{
			sfRenderWindow_drawSprite(_window, object[i].shadow, NULL);
		}
		sfRenderWindow_drawSprite(_window, object[i].sprite, NULL);
	}
}

void CleanupObject(void)
{
	for (int i = 0; i < objectCount; i++)
	{
		DeleteObject(i);
	}
	for (int i = 0; i < 5; i++)
	{
		sfTexture_destroy(textureObject[i]);
		textureObject[i] = NULL;
	}
}

void AddObject(sfVector2f _pos, float _rot, ObjectType _type)
{
	if (objectCount < 300)
	{
		Object obj = { 0 };
		obj.sprite = sfSprite_create();
		sfSprite_setTexture(obj.sprite, textureObject[_type], sfTrue);

		sfFloatRect hitbox = sfSprite_getGlobalBounds(obj.sprite);
		sfSprite_setOrigin(obj.sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
		sfSprite_setPosition(obj.sprite, _pos);

		obj.type = _type;

		if (_type == POT)
		{
			obj.shadow = sfSprite_create();
			sfSprite_setTexture(obj.shadow, shadowTexture, sfTrue);
			hitbox = sfSprite_getGlobalBounds(obj.shadow);
			sfSprite_setOrigin(obj.shadow, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
			sfSprite_setPosition(obj.shadow, (sfVector2f) { _pos.x, _pos.y - 15 });
			AddWall((sfVector2f) { _pos.x + 10 - hitbox.width / 2, _pos.y - hitbox.height / 2 + 10 }, 0, (sfVector2f) { hitbox.width - 20, hitbox.height - 25 });
			obj.wallID = GetWallCount();
			sfColor col = (sfColor){ 100, 255, 255, 255 };
			col.r += rand() % 199 + 1;
			sfSprite_setColor(obj.sprite, col);
		}
		else
		{
			AddWall((sfVector2f) { _pos.x - hitbox.width / 2, _pos.y - hitbox.height / 2 }, 0, (sfVector2f) { hitbox.width, hitbox.height });
			obj.wallID = GetWallCount();
		}

		object[objectCount] = obj;
		objectCount++;
	}
}

void DeleteObject(int _ID)
{
	sfSprite_destroy(object[_ID].sprite);
	object[_ID].sprite = NULL;
	if (object[_ID].type == POT)
	{
		sfSprite_destroy(object[_ID].shadow);
		object[_ID].shadow = NULL;
	}

	for (int i = _ID; i < objectCount - 1; i++)
	{
		Object temp = object[i];
		object[i] = object[i + 1];
		object[i + 1] = temp;
	}

	objectCount--;
}

void MoveObject(int _ID, sfVector2f _pos)
{
	sfSprite_setPosition(object[_ID].sprite, _pos);
}