#include "Shop.h"

sfTexture* textureItems[6];
sfFont* font;
sfText* description;
ItemShop itemShop[3];
int price[6];

sfBool test;

void LoadShop(void)
{
	textureItems[POTION] = sfTexture_createFromFile("Assets/Texture/Shop Items/potion.png", NULL);
	textureItems[BIG_POTION] = sfTexture_createFromFile("Assets/Texture/Shop Items/bigPotion.png", NULL);
	textureItems[SAW] = sfTexture_createFromFile("Assets/Texture/Shop Items/saw.png", NULL);
	textureItems[ANNOYING_ROCK] = sfTexture_createFromFile("Assets/Texture/Shop Items/rock.png", NULL);
	textureItems[BOOMERANG] = sfTexture_createFromFile("Assets/Texture/Shop Items/boomerang.png", NULL);
	textureItems[BLUE_GEL] = sfTexture_createFromFile("Assets/Texture/Shop Items/blue_gel.png", NULL);
	font = sfFont_createFromFile("Assets/Font/font.ttf");

	//Price
	price[POTION] = 10;
	price[BIG_POTION] = 20;
	price[SAW] = 50;
	price[ANNOYING_ROCK] = 0;
	price[BOOMERANG] = 50;
	price[BLUE_GEL] = 50;

	//Items
	for (int i = 0; i < 3; i++)
	{
		itemShop[i].sprite = sfSprite_create();

		itemShop[i].text = sfText_create();
		sfText_setFont(itemShop[i].text, font);
		sfText_setOutlineColor(itemShop[i].text, sfBlack);
		sfText_setOutlineThickness(itemShop[i].text, 1);
		sfText_setCharacterSize(itemShop[i].text, 50);
		sfText_setString(itemShop[i].text, "0");
	}
	description = sfText_create();
	sfText_setFont(description, font);
	sfText_setOutlineColor(description, sfBlack);
	sfText_setOutlineThickness(description, 1);
	sfText_setCharacterSize(description, 35);
	sfText_setString(description, "NULL");
}

void UpdateShop(float _dt, sfRenderWindow* _window)
{
	if (!test)
	{
		UpdateSlot(GetArtifactCount(), textureItems[BOOMERANG]);
		SetArtifact(BOOMERANG);
		UpdateSlot(GetArtifactCount(), textureItems[SAW]);
		SetArtifact(SAW);
		UpdateSlot(GetArtifactCount(), textureItems[BLUE_GEL]);
		SetArtifact(BLUE_GEL);
		test = sfTrue;
	}

	for (int i = 0; i < 3; i++)
	{
		ItemDescription();
		if (itemShop[i].isActive)
		{
			sfFloatRect hitbox = sfSprite_getGlobalBounds(itemShop[i].sprite);
			sfFloatRect pHitbox = GetPlayerHitbox();

			if (sfFloatRect_intersects(&hitbox, &pHitbox, NULL))
			{
				if (GetMoney() >= price[itemShop[i].type])
				{
					if (itemShop[i].type < 4)
					{
						if (GetArtifactCount() < 3)
						{
							itemShop[i].isActive = sfFalse;
							LoseGold(price[itemShop[i].type]);
							UpdateSlot(GetArtifactCount(), textureItems[itemShop[i].type]);
							SetArtifact(itemShop[i].type);
						}
					}
					else 
					{
						itemShop[i].isActive = sfFalse;
						LoseGold(price[itemShop[i].type]);
						switch (itemShop[i].type)
						{
						case POTION:
							GainLife(1);
							break;
						case BIG_POTION:
							GainLife(2);
							break;
						default:
							break;
						}
					}
				}
			}
		}


		if (GetMoney() >= price[itemShop[i].type])
		{
			sfText_setColor(itemShop[i].text, sfWhite);
			if (itemShop[i].type == SAW && GetArtifactCount() >= 3)
			{
				sfText_setColor(itemShop[i].text, sfRed);
			}
		}
		else
		{
			sfText_setColor(itemShop[i].text, sfRed);
		}
	}
}


void DrawShop(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < 3; i++)
	{
		if (itemShop[i].isActive)
		{
			sfRenderWindow_drawSprite(_window, itemShop[i].sprite, NULL);
			sfRenderWindow_drawText(_window, itemShop[i].text, NULL);
		}
	}
	sfRenderWindow_drawText(_window, description, NULL);
}

void CleanupShop(void)
{
	for (int i = 0; i < 5; i++)
	{
		sfTexture_destroy(textureItems[i]);
		textureItems[i] = NULL;
	}
	sfFont_destroy(font);
	font = NULL;
	for (int i = 0; i < 3; i++)
	{
		sfSprite_destroy(itemShop[i].sprite);
		itemShop[i].sprite = NULL;

		sfText_destroy(itemShop[i].text);
		itemShop[i].text = NULL;
	}
	sfText_destroy(description);
	description = NULL;
}

void AddItem(int _num, ShopType _type, sfVector2f _position)
{
	sfSprite_setTexture(itemShop[_num].sprite, textureItems[_type], sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(itemShop[_num].sprite);
	sfSprite_setOrigin(itemShop[_num].sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfSprite_setPosition(itemShop[_num].sprite, _position);
	itemShop[_num].type = _type;

	UpdateText(itemShop[_num].text, price[_type]);
	sfText_setPosition(itemShop[_num].text, (sfVector2f) { _position.x, _position.y + 30 });
	hitbox = sfText_getGlobalBounds(itemShop[_num].text);
	sfText_setOrigin(itemShop[_num].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	itemShop[_num].isActive = sfTrue;
}

void ItemDescription(void)
{
	sfVector2f pos = sfSprite_getPosition(itemShop[0].sprite);
	float closestPos = GetDistanceVector2f(pos, GetPlayerPos());
	sfVector2f finalPos = { 0 };
	int closest = 0;
	for (int i = 0; i < 3; i++)
	{
		pos = sfSprite_getPosition(itemShop[i].sprite);
		float newPos = GetDistanceVector2f(pos, GetPlayerPos());
		if (newPos <= closestPos)
		{
			closestPos = newPos;
			closest = i;
			finalPos = sfSprite_getPosition(itemShop[i].sprite);
		}
	}
	if (closestPos > 300 || !itemShop[closest].isActive)
	{
		sfText_setPosition(description, (sfVector2f) { 0, 0 });
	}
	else
	{
		switch (itemShop[closest].type)
		{
		case POTION:
			sfText_setString(description, "POTION\nGain half a heart");
			break;
		case BIG_POTION:
			sfText_setString(description, "BIG POTION\nGain a full heart");
			break;
		case SAW:
			sfText_setString(description, "SAW\nFire two more bullets on the sides");
			break;
		case ANNOYING_ROCK:
			sfText_setString(description, "ROCK?\nMaybe something is inside");
			break;
		case BOOMERANG:
			sfText_setString(description, "BOOMERANG\nBullets come back to deal more damage");
			break;
		case BLUE_GEL:
			sfText_setString(description, "BLUE GEL\nBullets bounce up walls twice and get faster");
			break;
		}
		sfFloatRect hitbox = sfText_getGlobalBounds(description);
		sfText_setPosition(description, (sfVector2f) { finalPos.x - hitbox.width / 2, finalPos.y + 120 });
	}
}