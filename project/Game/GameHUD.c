#include "GameHUD.h"

LifePoints lifePoints[3];
ArtifactSlot slot[3];
GameHUD gameHUD;
int currentLife;

sfTexture* lifeTexture[3];
sfTexture* textureSlot;
sfTexture* goldTexture;

sfBool hit[2];
float size[2];

//Bar
sfTexture* barTexture[3];
sfSprite* barSprite[3];
int maxBossHP[3];
float yellowBar;
float totalLength;

void LoadGameHUD(sfFont* font)
{
	font = sfFont_createFromFile("Assets/Font/font.ttf");
	currentLife = 0;

	//Life
	lifeTexture[0] = sfTexture_createFromFile("Assets/Texture/HUD/life.png", NULL);
	lifeTexture[1] = sfTexture_createFromFile("Assets/Texture/HUD/lifehalf.png", NULL);
	lifeTexture[2] = sfTexture_createFromFile("Assets/Texture/HUD/lifeempty.png", NULL);
	goldTexture = sfTexture_createFromFile("Assets/Texture/HUD/gold.png", NULL);
	textureSlot = sfTexture_createFromFile("Assets/Texture/HUD/slot.png", NULL);

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

	slot[0].sprite = sfSprite_create();
	sfSprite_setTexture(slot[0].sprite, textureSlot, sfTrue);
	hitbox = sfSprite_getGlobalBounds(lifePoints[0].sprite);
	sfSprite_setOrigin(slot[0].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(slot[0].sprite, (sfVector2f) { -720.f, 990.f });
	slot[1].sprite = sfSprite_create();
	sfSprite_setTexture(slot[1].sprite, textureSlot, sfTrue);
	hitbox = sfSprite_getGlobalBounds(lifePoints[1].sprite);
	sfSprite_setOrigin(slot[1].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(slot[1].sprite, (sfVector2f) { -540.f, 990.f });
	slot[2].sprite = sfSprite_create();
	sfSprite_setTexture(slot[2].sprite, textureSlot, sfTrue);
	hitbox = sfSprite_getGlobalBounds(lifePoints[2].sprite);
	sfSprite_setOrigin(slot[2].sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(slot[2].sprite, (sfVector2f) { -360.f, 990.f });

	GainLife(6);

	//Gold
	gameHUD.sprite = sfSprite_create();
	sfSprite_setTexture(gameHUD.sprite, goldTexture, sfTrue);
	hitbox = sfSprite_getGlobalBounds(gameHUD.sprite);
	sfSprite_setOrigin(gameHUD.sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
	sfSprite_setPosition(gameHUD.sprite, (sfVector2f) { -400, 150 });

	gameHUD.text = sfText_create();
	sfText_setFont(gameHUD.text, font);
	sfText_setOutlineColor(gameHUD.text, sfBlack);
	sfText_setOutlineThickness(gameHUD.text, 2);
	sfText_setCharacterSize(gameHUD.text, 50);
	sfText_setString(gameHUD.text, "0");
	sfText_setPosition(gameHUD.text, (sfVector2f) { -350, 110 });

	//Bossbar
	barTexture[0] = sfTexture_createFromFile("Assets/Texture/HUD/barBoss.png", NULL);
	barTexture[1] = sfTexture_createFromFile("Assets/Texture/HUD/yellowBar.png", NULL);
	barTexture[2] = sfTexture_createFromFile("Assets/Texture/HUD/redBar.png", NULL);

	for (int i = 0; i < 3; i++)
	{
		barSprite[i] = sfSprite_create();
		sfSprite_setTexture(barSprite[i], barTexture[i], sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(barSprite[i]);
		sfSprite_setOrigin(barSprite[i], (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
		sfSprite_setPosition(barSprite[i], (sfVector2f) { 500, 950 });
	}

	size[1] = 1;

	yellowBar = 942;
	totalLength = 942;
	maxBossHP[0] = 80;
	maxBossHP[1] = 200;
	maxBossHP[2] = 250;
}

void UpdateGameHUD(float _dt)
{
	//Visual stuff when you gain lose lives or money
	if (hit[0] && currentLife > 0)
	{
		sfSprite_setScale(lifePoints[currentLife - 1].sprite, (sfVector2f) { 1 * size[0], 1 * size[0] });
		if (size[0] > 1)
			size[0] -= 0.05f;
		else
			hit[0] = sfFalse;
	}

	if (hit[1])
	{
		sfText_setCharacterSize(gameHUD.text, 50 * size[1]);
		if (size[1] > 1)
			size[1] -= 0.05f;
		else
			hit[1] = sfFalse;
	}

	//Boss bar
	sfFloatRect hitbox = sfSprite_getGlobalBounds(barSprite[2]);
	float por = ((float)GetBossHP() / maxBossHP[GetBossActive()]) * 100;
	float dim = (por * totalLength) / 100;


	sfSprite_setTextureRect(barSprite[2], (sfIntRect) { 0, 0, (int)dim, (int)hitbox.height });
	if (yellowBar > dim)
	{
		yellowBar -= 0.5f;
	}
	sfSprite_setTextureRect(barSprite[1], (sfIntRect) { 0, 0, (int) yellowBar, (int)hitbox.height });
}

void DrawGameHUD(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < 3; i++)
	{
		sfRenderWindow_drawSprite(_window, lifePoints[i].sprite, NULL);
		sfRenderWindow_drawSprite(_window, slot[i].sprite, NULL);
	}

	sfRenderWindow_drawSprite(_window, gameHUD.sprite, NULL);
	sfRenderWindow_drawText(_window, gameHUD.text, NULL);

	if (GetBossID() == GetCurrentMap() && GetBossHP() > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			sfRenderWindow_drawSprite(_window, barSprite[i], NULL);
		}
	}
}

void CleanupGameHUD(sfFont* font)
{
	for (int i = 0; i < 3; i++)
	{
		sfSprite_destroy(lifePoints[i].sprite);
		lifePoints[i].sprite = NULL;
		sfSprite_destroy(slot[i].sprite);
		slot[i].sprite = NULL;
		sfTexture_destroy(lifeTexture[i]);
		lifeTexture[i] = NULL;
	}

	sfTexture_destroy(goldTexture);
	goldTexture = NULL;
	sfTexture_destroy(textureSlot);
	textureSlot = NULL;
	sfFont_destroy(font);
	font = NULL;

	//Other
	sfSprite_destroy(gameHUD.sprite);
	gameHUD.sprite = NULL;
	sfText_destroy(gameHUD.text);
	gameHUD.text = NULL;
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
	hit[0] = sfTrue;
	size[0] = 1.8f;
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

	hit[0] = sfTrue;
	size[0] = 1.8f;
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
	UpdateText(gameHUD.text, _value);
	hit[1] = sfTrue;
	if (size[1] < 2.5f)
	{
		size[1] += 0.3f;
	}
}

void UpdateSlot(int _ID, sfTexture* _texture)
{
	sfSprite_setTexture(slot[_ID].sprite, _texture, sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(slot[_ID].sprite);
	sfSprite_move(slot[_ID].sprite, (sfVector2f) { -(hitbox.width / 2), });
}

void ResetYellowBar(void)
{
	yellowBar = 942;
}
