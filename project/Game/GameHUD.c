#include "GameHUD.h"

LifePoints* lifePoints = NULL;
GameHUD gameHUD[1];
sfTexture* lifeTexture[2];
sfTexture* goldTexture;
int lifeCount;

sfFont* font;

void LoadGameHUD(void)
{
	font = sfFont_createFromFile("Assets/Font/font.ttf");

	//Life
	lifeCount = 0;
	lifePoints = calloc(1, sizeof(LifePoints));
	if (!lifePoints)
	{
		fprintf(stderr, "Erreur d'allocation memoire pour enemyList.\n");
		return;
	}
	lifeTexture[0] = sfTexture_createFromFile("Assets/Texture/HUD/life.png", NULL);
	lifeTexture[1] = sfTexture_createFromFile("Assets/Texture/HUD/lifehalf.png", NULL);
	goldTexture = sfTexture_createFromFile("Assets/Texture/HUD/gold.png", NULL);
	GainLife(6);

	//Gold
	gameHUD[0].sprite = sfSprite_create();
	sfSprite_setTexture(gameHUD[0].sprite, goldTexture, sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(gameHUD[0].sprite);
	sfSprite_setOrigin(gameHUD[0].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(gameHUD[0].sprite, (sfVector2f) { -400, 150 });

	gameHUD[0].text = sfText_create();
	sfText_setFont(gameHUD[0].text, font);
	sfText_setOutlineColor(gameHUD[0].text, sfBlack);
	sfText_setOutlineThickness(gameHUD[0].text, 2);
	sfText_setCharacterSize(gameHUD[0].text, 50);
	sfText_setString(gameHUD[0].text, "0");
	sfText_setPosition(gameHUD[0].text, (sfVector2f) { -350, 110 });
}

void UpdateGameHUD(float _dt)
{
	
}

void DrawGameHUD(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < lifeCount; i++)
	{
		sfRenderWindow_drawSprite(_window, lifePoints[i].sprite, NULL);
	}
	sfRenderWindow_drawSprite(_window, gameHUD[0].sprite, NULL);
	sfRenderWindow_drawText(_window, gameHUD[0].text, NULL);
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
	sfTexture_destroy(goldTexture);
	goldTexture = NULL;
	sfFont_destroy(font);
	font = NULL;

	//Other
	sfSprite_destroy(gameHUD[0].sprite);
	gameHUD[0].sprite = NULL;
	sfText_destroy(gameHUD[0].text);
	gameHUD[0].text = NULL;
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
		sfSprite_setPosition(life.sprite, (sfVector2f) { (float)(100 * lifeCount) - 350.f, 50.f });

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

void UpdateText(sfText* const _text, int _value)
{
	// Copy the value into the text
	char string[5];
	sprintf_s(string, sizeof(string), "%d", _value);
	sfText_setString(_text, string);
}

void UpdateGold(int _value)
{
	UpdateText(gameHUD[0].text, _value);
}