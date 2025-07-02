#include "Money.h"

Nugget nugget[100];
sfTexture* nuggetTexture;
int nuggetCount;

void LoadMoney(void)
{
	nuggetCount = 0;
	nuggetTexture = sfTexture_createFromFile("Assets/Texture/Objects/nugget.png",NULL);
}

void UpdateMoney(float _dt, sfRenderWindow* _window)
{
	for (int i = 0; i < nuggetCount; i++)
	{
		sfFloatRect hitbox = sfSprite_getGlobalBounds(nugget[i].sprite);
		sfFloatRect pHitbox = GetPlayerHitbox();
		sfVector2f pos = sfSprite_getPosition(nugget[i].sprite);

		//Range
		if (GetDistanceVector2f(GetPlayerPos(), pos) < 200)
		{
			float rotation = LookToDirection(GetPlayerPos(), pos) + 90;
			float radian = rotation * (float)(M_PI / 180);
			nugget[i].velocity.x = (float)(sin(radian));
			nugget[i].velocity.y = (float)(-cos(radian));
			if (nugget[i].speed < 700)
			{
				nugget[i].speed += 20;
			}
		}
		else if (nugget[i].speed > 0)
		{
			nugget[i].speed -= 20;
		}
		sfSprite_move(nugget[i].sprite, (sfVector2f) { nugget[i].velocity.x* nugget[i].speed * _dt, nugget[i].velocity.y* nugget[i].speed * _dt });

		//Collect
		if (sfFloatRect_intersects(&hitbox, &pHitbox, NULL))
		{
			DeleteNugget(i);
		}
	}
}

void DrawMoney(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < nuggetCount; i++)
	{
		sfRenderWindow_drawSprite(_window, nugget[i].sprite, NULL);
	}
}

void CleanupMoney(void)
{
	for (int i = 0; i < nuggetCount; i++)
	{
		DeleteNugget(i);
	}
	sfTexture_destroy(nuggetTexture);
	nuggetTexture = NULL;
}

void AddNugget(sfFloatRect _hitbox)
{
	if (nuggetCount < 100)
	{
		Nugget temp = { 0 };

		temp.sprite = sfSprite_create();
		sfSprite_setTexture(temp.sprite, nuggetTexture, sfTrue);
		int randLeft = rand() % (int)_hitbox.width;
		int randTop = rand() % (int)_hitbox.height;
		sfSprite_setPosition(temp.sprite, (sfVector2f) { _hitbox.left + (float)randLeft, _hitbox.top + (float)randTop });

		sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
		sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		nugget[nuggetCount] = temp;
		nuggetCount++;
	}
}

void DeleteNugget(int _ID)
{
	sfSprite_destroy(nugget[_ID].sprite);
	nugget[_ID].sprite = NULL;

	for (int i = _ID; i < nuggetCount - 1; i++)
	{
		Nugget temp = nugget[i];
		nugget[i] = nugget[i + 1];
		nugget[i + 1] = temp;
	}

	nuggetCount--;
}