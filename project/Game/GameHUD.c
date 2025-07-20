#include "GameHUD.h"

LifePoints lifePoints[3];
GameHUD gameHUD[1];
int currentLife;

sfTexture* lifeTexture[3];
sfTexture* goldTexture;
sfFont* font;

void LoadGameHUD(void)
{
	font = sfFont_createFromFile("Assets/Font/font.ttf");
	currentLife = 0;

	//Life
	lifeTexture[0] = sfTexture_createFromFile("Assets/Texture/HUD/life.png", NULL);
	lifeTexture[1] = sfTexture_createFromFile("Assets/Texture/HUD/lifehalf.png", NULL);
	lifeTexture[2] = sfTexture_createFromFile("Assets/Texture/HUD/lifeempty.png", NULL);
	goldTexture = sfTexture_createFromFile("Assets/Texture/HUD/gold.png", NULL);

	lifePoints[0].sprite = sfSprite_create();
	sfSprite_setTexture(lifePoints[0].sprite, lifeTexture[2], sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(lifePoints[0].sprite);
	sfSprite_setOrigin(lifePoints[0].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(lifePoints[0].sprite, (sfVector2f) { (float)(100 * 0) - 350.f, 50.f });
	lifePoints[1].sprite = sfSprite_create();
	sfSprite_setTexture(lifePoints[1].sprite, lifeTexture[2], sfTrue);
	hitbox = sfSprite_getGlobalBounds(lifePoints[1].sprite);
	sfSprite_setOrigin(lifePoints[1].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(lifePoints[1].sprite, (sfVector2f) { (float)(100 * 1) - 350.f, 50.f });
	lifePoints[2].sprite = sfSprite_create();
	sfSprite_setTexture(lifePoints[2].sprite, lifeTexture[2], sfTrue);
	hitbox = sfSprite_getGlobalBounds(lifePoints[2].sprite);
	sfSprite_setOrigin(lifePoints[2].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(lifePoints[2].sprite, (sfVector2f) { (float)(100 * 2) - 350.f, 50.f });

	GainLife(6);

	//Gold
	gameHUD[0].sprite = sfSprite_create();
	sfSprite_setTexture(gameHUD[0].sprite, goldTexture, sfTrue);
	hitbox = sfSprite_getGlobalBounds(gameHUD[0].sprite);
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
	sfRenderWindow_drawSprite(_window, lifePoints[0].sprite, NULL);
	sfRenderWindow_drawSprite(_window, lifePoints[1].sprite, NULL);
	sfRenderWindow_drawSprite(_window, lifePoints[2].sprite, NULL);

	sfRenderWindow_drawSprite(_window, gameHUD[0].sprite, NULL);
	sfRenderWindow_drawText(_window, gameHUD[0].text, NULL);
}

void CleanupGameHUD(void)
{
	sfSprite_destroy(lifePoints[0].sprite);
	lifePoints[0].sprite = NULL;
	sfSprite_destroy(lifePoints[1].sprite);
	lifePoints[1].sprite = NULL;
	sfSprite_destroy(lifePoints[2].sprite);
	lifePoints[2].sprite = NULL;

	sfTexture_destroy(lifeTexture[0]);
	lifeTexture[0] = NULL;
	sfTexture_destroy(lifeTexture[1]);
	lifeTexture[1] = NULL;
	sfTexture_destroy(lifeTexture[2]);
	lifeTexture[2] = NULL;
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

void AddLife(void)
{
	if (GetHP() % 2 != 0)
	{
		currentLife++;
		sfSprite_setTexture(lifePoints[currentLife - 1].sprite, lifeTexture[1], sfTrue);
	}
	else
	{
		sfSprite_setTexture(lifePoints[currentLife - 1].sprite, lifeTexture[0], sfTrue);
	}
}

void DeleteLife(void)
{
	if (GetHP() % 2 == 0)
	{
		sfSprite_setTexture(lifePoints[currentLife - 1].sprite, lifeTexture[2], sfTrue);
		currentLife--;
	}
	else
	{
		sfSprite_setTexture(lifePoints[currentLife - 1].sprite, lifeTexture[1], sfTrue);
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