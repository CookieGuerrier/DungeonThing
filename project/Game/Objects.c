#include "Objects.h"

Object object[300];
sfTexture* textureObject[11];
sfTexture* textureShadow;
int objectCount;

int wall1, wall2, wall3, wall4;

sfBool exitActive;

void LoadObject(void)
{
	exitActive = sfFalse;
	objectCount = 0;
	textureObject[WALL_BLOCKUD] = sfTexture_createFromFile("Assets/Texture/Objects/wallUD.png", NULL);
	textureObject[WALL_BLOCKRL] = sfTexture_createFromFile("Assets/Texture/Objects/wallRL.png", NULL);
	textureObject[WALL_BATTLEUD] = sfTexture_createFromFile("Assets/Texture/Objects/blockUD.png", NULL);
	textureObject[WALL_BATTLERL] = sfTexture_createFromFile("Assets/Texture/Objects/blockRL.png", NULL);

	textureObject[POT] = sfTexture_createFromFile("Assets/Texture/Objects/pot.png", NULL);
	textureObject[STATUE] = sfTexture_createFromFile("Assets/Texture/Objects/shop.png", NULL);
	textureObject[BIG_HOLE] = sfTexture_createFromFile("Assets/Texture/Objects/bigHole.png", NULL);
	textureObject[LONG_HOLE] = sfTexture_createFromFile("Assets/Texture/Objects/longHole.png", NULL);
	textureObject[ROCK] = sfTexture_createFromFile("Assets/Texture/Objects/rock.png", NULL);
	textureObject[TORCH] = sfTexture_createFromFile("Assets/Texture/Objects/torch.png", NULL);
	textureObject[EXIT_HOLE] = sfTexture_createFromFile("Assets/Texture/Objects/exit.png", NULL);
	textureShadow = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);

	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLERL);
	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLERL);
	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLEUD);
	AddObject((sfVector2f) { 0, 0 }, 0, WALL_BATTLEUD);
}

void UpdateObject(float _dt, sfRenderWindow* _window)
{
	for (int i = 0; i < objectCount; i++)
	{
		if (object[i].type == TORCH)
		{
			UpdateAnim(_dt, object[i].anims[0]);
		}
		else if (object[i].type == EXIT_HOLE)
		{
			if (exitActive)
			{
				UpdateAnim(_dt, object[i].anims[0]);
				sfFloatRect hitbox = sfSprite_getGlobalBounds(object[i].sprite);
				sfFloatRect hitbox2 = GetPlayerHitbox();
				if (sfFloatRect_intersects(&hitbox, &hitbox2, NULL))
				{
					SetMapCreation();
				}
			}
		}
	}
}

void DrawObject(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < objectCount; i++)
	{
		if (object[i].type == POT || object[i].type == ROCK)
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
		objectCount++;
	}

	for (int i = 0; i < 11; i++)
	{
		sfTexture_destroy(textureObject[i]);
		textureObject[i] = NULL;
	}
	sfTexture_destroy(textureShadow);
	textureShadow = NULL;
}

void AddObject(sfVector2f _pos, float _rot, ObjectType _type)
{
	if (objectCount < 300)
	{
		Object obj = { 0 };
		obj.sprite = sfSprite_create();
		int ran = 0;
		sfSprite_setTexture(obj.sprite, textureObject[_type], sfTrue);

		sfFloatRect hitbox = sfSprite_getGlobalBounds(obj.sprite);
		sfSprite_setOrigin(obj.sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
		sfSprite_setPosition(obj.sprite, _pos);

		obj.type = _type;

		if (_type == WALL_BATTLERL || _type == WALL_BATTLEUD || _type == WALL_BLOCKRL || _type == WALL_BLOCKUD)
		{
			AddWall((sfVector2f) { _pos.x - hitbox.width / 2, _pos.y - hitbox.height / 2 }, 0, (sfVector2f) { hitbox.width, hitbox.height }, sfFalse, objectCount);
			obj.wallID = GetWallCount();
		}

		switch (_type)
		{
		case POT:
			obj.shadow = sfSprite_create();
			sfSprite_setTexture(obj.shadow, textureShadow, sfTrue);
			hitbox = sfSprite_getGlobalBounds(obj.shadow);
			sfSprite_setOrigin(obj.shadow, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
			sfSprite_setPosition(obj.shadow, (sfVector2f) { _pos.x, _pos.y - 15 });
			AddWall((sfVector2f) { _pos.x + 10 - hitbox.width / 2, _pos.y - hitbox.height / 2 + 10 }, 0, (sfVector2f) { hitbox.width - 20, hitbox.height - 25 }, sfFalse, objectCount);
			obj.wallID = GetWallCount();
			sfColor col = (sfColor){ 100, 255, 255, 255 };
			col.r += rand() % 199 + 1;
			sfSprite_setColor(obj.sprite, col);
			break;
		case STATUE:
			AddWall((sfVector2f) { _pos.x - hitbox.width / 2, _pos.y - hitbox.height / 2 }, 0, (sfVector2f) { hitbox.width, hitbox.height - 60 }, sfFalse, objectCount);
			break;
		case BIG_HOLE:
			AddWall((sfVector2f) { _pos.x - hitbox.width / 2, _pos.y - hitbox.height / 2 }, 0, (sfVector2f) { hitbox.width, hitbox.height }, sfTrue, objectCount);
			break;
		case LONG_HOLE:
			AddWall((sfVector2f) { _pos.x - hitbox.width / 2, _pos.y - hitbox.height / 2 }, 0, (sfVector2f) { hitbox.width, hitbox.height }, sfTrue, objectCount);
			break;
		case ROCK:
			ran = rand() % 3;
			sfSprite_setTextureRect(obj.sprite, (sfIntRect) { 0 + 60 * ran, 0, 60, 60 });
			obj.shadow = sfSprite_create();
			sfSprite_setTexture(obj.shadow, textureShadow, sfTrue);
			sfSprite_setScale(obj.shadow, (sfVector2f) { 0.9f, 1 });
			hitbox = sfSprite_getGlobalBounds(obj.shadow);
			sfSprite_setOrigin(obj.shadow, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
			sfSprite_setPosition(obj.shadow, (sfVector2f) { _pos.x - 65, _pos.y - 5 });
			AddWall((sfVector2f) { _pos.x - hitbox.width / 2 - 60, _pos.y - hitbox.height / 2 }, 0, (sfVector2f) { hitbox.width, hitbox.height }, sfFalse, objectCount);
			break;
		case TORCH:
			obj.anims = calloc(4, sizeof(Anim*));
			if (obj.anims != NULL)
			{
				sfVector2u size = sfTexture_getSize(textureObject[TORCH]);
				sfIntRect first = { 0, 0, size.x / 3, size.y };
				obj.anims[0] = CreateAnim(textureObject[TORCH], first, 3, 1 / 6.0f, obj.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 8.f }, sfTrue);
				obj.anims[0]->aimOffset = (sfVector2f){ 0 };
				obj.anims[0]->events = malloc(sizeof(AnimEvent));
			}
			break;
		case EXIT_HOLE:
			obj.anims = calloc(4, sizeof(Anim*));
			if (obj.anims != NULL)
			{
				sfVector2u size = sfTexture_getSize(textureObject[EXIT_HOLE]);
				sfIntRect first = { 0, 0, size.x / 6, size.y };
				obj.anims[0] = CreateAnim(textureObject[EXIT_HOLE], first, 6, 1 / 12.0f, obj.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 8.f }, sfFalse);
				obj.anims[0]->aimOffset = (sfVector2f){ 0 };
				obj.anims[0]->events = malloc(sizeof(AnimEvent));
			}
			break;
		}

		object[objectCount] = obj;
		objectCount++;
	}
}

void DeleteObject(int _ID)
{
	sfSprite_destroy(object[_ID].sprite);
	object[_ID].sprite = NULL;
	if (object[_ID].type == POT || object[_ID].type == ROCK)
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

void ClearObject(void)
{
	for (int i = 4; i < objectCount; i++)
	{
		DeleteObject(i);
		objectCount++;
	}
	objectCount = 4;
	exitActive = sfFalse;
}

void MoveObject(int _ID, sfVector2f _pos)
{
	sfSprite_setPosition(object[_ID].sprite, _pos);

	if (object[_ID].type == WALL_BATTLERL || object[_ID].type == WALL_BATTLEUD)
	{
		sfVector2f pos = sfSprite_getPosition(object[_ID].sprite);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(object[_ID].sprite);
		MoveWall(_ID, (sfVector2f) { pos.x - hitbox.width / 2, pos.y - hitbox.height / 2 });
	}
}

void SetExitActive(void)
{
	exitActive = sfTrue;
}
