#include "Shop.h"

sfTexture* textureItems[2];
sfFont* font;
ItemShop itemShop[3];
int price[2];

void LoadShop(void)
{
	textureItems[POTION] = sfTexture_createFromFile("Assets/Texture/Shop Items/potion.png", NULL);
	textureItems[BIG_POTION] = sfTexture_createFromFile("Assets/Texture/Shop Items/bigPotion.png", NULL);
	font = sfFont_createFromFile("Assets/Font/font.ttf");

	//Price
	price[POTION] = 10;
	price[BIG_POTION] = 20;

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
}

void UpdateShop(float _dt, sfRenderWindow* _window)
{
	for (int i = 0; i < 3; i++)
	{
		if (itemShop[i].isActive)
		{
			sfFloatRect hitbox = sfSprite_getGlobalBounds(itemShop[i].sprite);
			sfFloatRect pHitbox = GetPlayerHitbox();

			if (sfFloatRect_intersects(&hitbox, &pHitbox, NULL))
			{
				if (GetMoney() >= price[itemShop[i].type])
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

			if (GetMoney() >= price[itemShop[i].type])
			{
				sfText_setColor(itemShop[i].text, sfWhite);
			}
			else
			{
				sfText_setColor(itemShop[i].text, sfRed);
			}
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
}

void CleanupShop(void)
{
	sfTexture_destroy(textureItems[0]);
	textureItems[0] = NULL;
	for (int i = 0; i < 3; i++)
	{
		sfSprite_destroy(itemShop[i].sprite);
		itemShop[i].sprite = NULL;

		sfText_destroy(itemShop[i].text);
		itemShop[i].text = NULL;
	}
}

void AddItem(int _num, ShopType _type, sfVector2f _position)
{
	sfSprite_setTexture(itemShop[_num].sprite, textureItems[_type], sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(itemShop[_num].sprite);
	sfSprite_setOrigin(itemShop[_num].sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfSprite_setPosition(itemShop[_num].sprite, _position);
	itemShop[_num].type = _type;

	UpdateText(itemShop[_num].text, price[_type]);
	sfText_setPosition(itemShop[_num].text, (sfVector2f) { _position.x, _position.y + 30});
	hitbox = sfText_getGlobalBounds(itemShop[_num].text);
	sfText_setOrigin(itemShop[_num].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	itemShop[_num].isActive = sfTrue;
}