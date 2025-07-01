#include "GameHUD.h"

LifePoints* lifePoints = NULL;
sfTexture* lifeTexture[2];
int lifeCount;

void LoadGameHUD(void)
{
	lifeCount = 0;
	lifePoints = calloc(1, sizeof(LifePoints));
	if (!lifePoints)
	{
		fprintf(stderr, "Erreur d'allocation memoire pour enemyList.\n");
		return;
	}
	lifeTexture[0] = sfTexture_createFromFile("Assets/Texture/HUD/life.png", NULL);
	lifeTexture[1] = sfTexture_createFromFile("Assets/Texture/HUD/lifehalf.png", NULL);
	GainLife(6);
}

void UpdateGameHUD(float _dt)
{
	for (int i = 0; i < lifeCount; i++)
	{
		sfSprite_setPosition(lifePoints[i].sprite, (sfVector2f) { (float)(100 * i) - 350.f, 50.f });
	}
}

void DrawGameHUD(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < lifeCount; i++)
	{
		sfRenderWindow_drawSprite(_window, lifePoints[i].sprite, NULL);
	}
}

void CleanupGameHUD(void)
{
	for (int i = 0; i < lifeCount; i++)
	{
		DeleteLifePoint();
	}
	sfTexture_destroy(lifeTexture[0]);
	lifeTexture[0] = NULL;
	sfTexture_destroy(lifeTexture[1]);
	lifeTexture[1] = NULL;
}

void AddLifePoint(void)
{
	if (GetHP() % 2 != 0)
	{
		LifePoints* temp = realloc(lifePoints, (size_t)(lifeCount + 1) * sizeof(LifePoints));
		if (!temp) {
			fprintf(stderr, "LifePoint list reallocation failed!\n");
			return;
		}
		lifePoints = temp;

		LifePoints life = { 0 };
		life.sprite = sfSprite_create();
		sfSprite_setTexture(life.sprite, lifeTexture[1], sfTrue);

		sfFloatRect hitbox = sfSprite_getGlobalBounds(life.sprite);
		sfSprite_setOrigin(life.sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });

		lifePoints[lifeCount] = life;
		lifeCount++;
	}
	else
	{
		if (lifeCount != 0)
		{
			sfSprite_setTexture(lifePoints[lifeCount - 1].sprite, lifeTexture[0], sfTrue);
		}
	}
}

void DeleteLifePoint(void)
{
	//Delete full heart
	if (GetHP() % 2 == 0)
	{
		if (lifeCount > 0)
		{
			if (lifeCount > 1)
			{
				LifePoints* temp = realloc(lifePoints, (size_t)(lifeCount - 1) * sizeof(LifePoints));
				if (!temp) {
					fprintf(stderr, "LifePoint list reallocation failed!\n");
					return;
				}

				lifePoints = temp;
				lifeCount--;
			}
			else
			{
				sfSprite_destroy(lifePoints[0].sprite);
				lifePoints[0].sprite = NULL;
				lifeCount--;
			}
		}
	}
	else
	{
		if (lifeCount != 0)
		{
			sfSprite_setTexture(lifePoints[lifeCount - 1].sprite, lifeTexture[1], sfTrue);
		}
	}
}
