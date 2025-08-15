#include "Shop.h"

sfTexture* textureItems[11];
sfFont* font;
sfText* description;
ItemShop itemShop[6];
int itemCount;
int price[11];

sfBool test;

void LoadShop(void)
{
	itemCount = 0;
	textureItems[POTION] = sfTexture_createFromFile("Assets/Texture/Shop Items/potion.png", NULL);
	textureItems[BIG_POTION] = sfTexture_createFromFile("Assets/Texture/Shop Items/bigPotion.png", NULL);
	textureItems[SAW] = sfTexture_createFromFile("Assets/Texture/Shop Items/saw.png", NULL);
	textureItems[CONTRACT] = sfTexture_createFromFile("Assets/Texture/Shop Items/contract.png", NULL);
	textureItems[BOOMERANG] = sfTexture_createFromFile("Assets/Texture/Shop Items/boomerang.png", NULL);
	textureItems[BLUE_GEL] = sfTexture_createFromFile("Assets/Texture/Shop Items/blue_gel.png", NULL);
	textureItems[CHRONO] = sfTexture_createFromFile("Assets/Texture/Shop Items/chrono.png", NULL);
	textureItems[STAMP] = sfTexture_createFromFile("Assets/Texture/Shop Items/stamp.png", NULL);
	textureItems[BATTERY] = sfTexture_createFromFile("Assets/Texture/Shop Items/battery.png", NULL);
	textureItems[PASSPORT] = sfTexture_createFromFile("Assets/Texture/Shop Items/passport.png", NULL);
	textureItems[COWBOY_HAT] = sfTexture_createFromFile("Assets/Texture/Shop Items/cowboy_hat.png", NULL);
	font = sfFont_createFromFile("Assets/Font/font.ttf");

	//Price
	price[POTION] = 10;
	price[BIG_POTION] = 20;
	price[SAW] = 50;
	price[CONTRACT] = 50;
	price[BOOMERANG] = 50;
	price[BLUE_GEL] = 50;
	price[CHRONO] = 50;
	price[STAMP] = 50;
	price[BATTERY] = 50;
	price[PASSPORT] = 50;
	price[COWBOY_HAT] = 50;

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
		//UpdateSlot(GetArtifactCount(), textureItems[CHRONO]);
		//SetArtifact(CHRONO);
		UpdateSlot(GetArtifactCount(), textureItems[COWBOY_HAT]);
		SetArtifact(COWBOY_HAT);
		UpdateSlot(GetArtifactCount(), textureItems[SAW]);
		SetArtifact(SAW);
		test = sfTrue;
	}

	for (int i = 0; i < itemCount; i++)
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
					if (itemShop[i].type < 9)
					{
						if (GetArtifactCount() < 3 && !GetEffect(itemShop[i].type))
						{
							itemShop[i].isActive = sfFalse;
							if (!itemShop[i].free)
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
							GainLife(2);
							break;
						case BIG_POTION:
							GainLife(4);
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
			if (itemShop[i].type < POTION && GetArtifactCount() >= 3)
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
	for (int i = 0; i < itemCount; i++)
	{
		if (itemShop[i].isActive)
		{
			sfRenderWindow_drawSprite(_window, itemShop[i].sprite, NULL);
			if (!itemShop[i].free)
			{
				sfRenderWindow_drawText(_window, itemShop[i].text, NULL);
			}
		}
	}
	sfRenderWindow_drawText(_window, description, NULL);
}

void CleanupShop(void)
{
	for (int i = 0; i < 8; i++)
	{
		sfTexture_destroy(textureItems[i]);
		textureItems[i] = NULL;
	}
	sfFont_destroy(font);
	font = NULL;
	for (int i = 0; i < itemCount; i++)
	{
		DeleteItem(i);
		itemCount++;
	}
	sfText_destroy(description);
	description = NULL;
}

void AddItem(ShopType _type, sfVector2f _position, sfBool _free)
{
	ItemShop temp = { 0 };
	temp.sprite = sfSprite_create();
	sfSprite_setTexture(temp.sprite, textureItems[_type], sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
	sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfSprite_setPosition(temp.sprite, _position);
	temp.type = _type;

	temp.text = sfText_create();
	sfText_setFont(temp.text, font);
	sfText_setOutlineColor(temp.text, sfBlack);
	sfText_setOutlineThickness(temp.text, 1);
	sfText_setCharacterSize(temp.text, 50);
	sfText_setString(temp.text, "0");

	UpdateText(temp.text, price[_type]);
	sfText_setPosition(temp.text, (sfVector2f) { _position.x, _position.y + 30 });
	hitbox = sfText_getGlobalBounds(temp.text);
	sfText_setOrigin(temp.text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	temp.isActive = sfTrue;
	
	temp.free = _free;

	itemShop[itemCount] = temp;
	itemCount++;
}

void DeleteItem(int _ID)
{
	sfSprite_destroy(itemShop[_ID].sprite);
	itemShop[_ID].sprite = NULL;

	sfText_destroy(itemShop[_ID].text);
	itemShop[_ID].text = NULL;
	itemCount--;
}

void ClearShop(void)
{
	for (int i = 0; i < itemCount; i++)
	{
		DeleteItem(i);
		itemCount++;
	}
	itemCount = 0;
}

void ItemDescription(void)
{
	sfVector2f pos = sfSprite_getPosition(itemShop[0].sprite);
	float closestPos = GetDistanceVector2f(pos, GetPlayerPos());
	sfVector2f finalPos = { 0 };
	int closest = 0;
	for (int i = 0; i < itemCount; i++)
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
			sfText_setString(description, "POTION\nHeals a heart");
			break;
		case BIG_POTION:
			sfText_setString(description, "BIG POTION\n Heals two hearts");
			break;
		case SAW:
			sfText_setString(description, "SAW\nFire two more bullets on the sides");
			break;
		case CONTRACT:
			sfText_setString(description, "CONTRACT\nYou do more damage with less hearts, but less with more hearts. \nYou also gain half a heart every time you hit an enemy 20 times.");
			break;
		case BOOMERANG:
			sfText_setString(description, "BOOMERANG\nBullets come back to deal more damage");
			break;
		case BLUE_GEL:
			sfText_setString(description, "BLUE GEL\nBullets bounce up walls thrice and get faster");
			break;
		case CHRONO:
			sfText_setString(description, "CHRONO\nDoes more damage depending on bullet speed");
			break;
		case STAMP:
			sfText_setString(description, "STAMP\nWhen taking damage, your fire rate quadruples for a few seconds");
			break;
		case BATTERY:
			sfText_setString(description, "BATTERY\nNeed to hold the fire button to shoot bullets\nBullets are bigger, faster and deal more damage");
			break;
		case PASSPORT:
			sfText_setString(description, "PASSPORT\nBullets can go through enemies, but it will never deal more than 1 damage");
			break;
		case COWBOY_HAT:
			sfText_setString(description, "COWBOY HAT\nHitting an enemy close enough will attach a rope to them\nRoped enemies won't be able to move and receive double damage");
			break;
		}
		sfFloatRect hitbox = sfText_getGlobalBounds(description);
		if (!itemShop[closest].free)
			sfText_setPosition(description, (sfVector2f) { finalPos.x - hitbox.width / 2, finalPos.y + 120 });
		else
			sfText_setPosition(description, (sfVector2f) { finalPos.x - hitbox.width / 2, finalPos.y + 40 });
	}
}