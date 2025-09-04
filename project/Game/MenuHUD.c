#include "MenuHUD.h"

sfTexture* backgroundText;
sfSprite* background;

int selection;
int pastSelection;
sfFont* font;
Buttons button[4];
sfTexture* selectionText;
Selection select[2];
sfVector2f mousePos;

int leafCount;
Leaf leaf[20];
float leafTimer;

void LoadMenuHUD(void)
{
	selection = 0;
	leafCount = 0;
	font = sfFont_createFromFile("Assets/Font/font.ttf");
	backgroundText = sfTexture_createFromFile("Assets/Texture/HUD/background.png", NULL);
	selectionText = sfTexture_createFromFile("Assets/Texture/HUD/hudMarker.png", NULL);

	background = sfSprite_create();
	sfSprite_setTexture(background, backgroundText, sfTrue);

	for (int i = 0; i < 4; i++)
	{
		button[i].text = sfText_create();
		sfText_setFont(button[i].text, font);
		sfText_setOutlineColor(button[i].text, sfBlack);
		sfText_setOutlineThickness(button[i].text, 2);
		sfText_setCharacterSize(button[i].text, 50);
	}
	sfText_setString(button[0].text, "New Game");
	sfFloatRect hitbox = sfText_getGlobalBounds(button[0].text);
	sfText_setOrigin(button[0].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[0].text, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	sfText_setString(button[1].text, "Continue");
	hitbox = sfText_getGlobalBounds(button[1].text);
	sfText_setOrigin(button[1].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[1].text, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100 });
	sfText_setColor(button[1].text, (sfColor) { 255, 255, 255, 120 });

	sfText_setString(button[2].text, "Options");
	hitbox = sfText_getGlobalBounds(button[2].text);
	sfText_setOrigin(button[2].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[2].text, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200 });
	sfText_setColor(button[2].text, (sfColor) { 255, 255, 255, 120 });

	sfText_setString(button[3].text, "Quit");
	hitbox = sfText_getGlobalBounds(button[3].text);
	sfText_setOrigin(button[3].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[3].text, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 300 });

	for (int i = 0; i < 2; i++)
	{
		select[i].sprite = sfSprite_create();
		sfSprite_setTexture(select[i].sprite, selectionText, sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(select[i].sprite);
		sfSprite_setOrigin(select[i].sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	}
	sfSprite_setScale(select[0].sprite, (sfVector2f) { -1, 1 });

	MoveCamera((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, 2);
	SetCamera((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
}

void UpdateMenuHUD(float _dt, sfRenderWindow* _window)
{
	//Menu
	sfVector2f pos = sfText_getPosition(button[selection].text);
	sfFloatRect hitbox = sfText_getGlobalBounds(button[selection].text);
	sfSprite_setPosition(select[0].sprite, (sfVector2f) { pos.x - hitbox.width / 2 - 20, pos.y + 25 });
	sfSprite_setPosition(select[1].sprite, (sfVector2f) { pos.x + hitbox.width / 2 + 20, pos.y + 25 });

	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	mousePos = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetView());
	for (int i = 0; i < 4; i++)
	{
		sfFloatRect hit = sfText_getGlobalBounds(button[i].text);
		if (sfFloatRect_contains(&hit, mousePos.x, mousePos.y))
		{
			selection = i;
		}
	}

	//Sound
	if (pastSelection != selection)
	{
		pastSelection = selection;
		PlaySound(0);
	}

	//Visual
	if (leafTimer <= 0)
	{
		float x = (float)(rand() % 1300 + 350);
		AddLeaf((sfVector2f) { x, 100 });
		leafTimer = 0.8f;
	}
	else
	{
		leafTimer -= _dt;
	}

	for (int i = 0; i < leafCount; i++)
	{
		float x = 0;
		if (leaf[i].turningLeft)
		{
			if (leaf[i].x > -2)
				leaf[i].x -= 0.02f;
			else
			{
				leaf[i].turningLeft = sfFalse;
				leaf[i].slow = sfTrue;
			}
		}
		else
		{
			if (leaf[i].x < 2)
				leaf[i].x += 0.02f;
			else
			{
				leaf[i].turningLeft = sfTrue;
				leaf[i].slow = sfTrue;
			}
		}
		
		if (!leaf[i].slow)
		{
			if (leaf[i].y < 2)
				leaf[i].y += 0.02f;
		}
		else
		{
			if (leaf[i].y > 1)
				leaf[i].y -= 0.02f;
			else
				leaf[i].slow = sfFalse;
		}

		sfRectangleShape_move(leaf[i].sprite, (sfVector2f) { leaf[i].x, leaf[i].y });
		if (sfRectangleShape_getPosition(leaf[i].sprite).y > 1000)
		{
			DeleteLeaf(i);
		}
	}
}

void DrawMenuHUD(sfRenderWindow* _window)
{
	sfRenderWindow_drawSprite(_window, background, NULL);
	for (int i = 0; i < 4; i++)
	{
		sfRenderWindow_drawText(_window, button[i].text, NULL);
	}
	for (int i = 0; i < 2; i++)
	{
		sfRenderWindow_drawSprite(_window, select[i].sprite, NULL);
	}
	for (int i = 0; i < leafCount; i++)
	{
		sfRenderWindow_drawRectangleShape(_window, leaf[i].sprite, NULL);
	}
}

void CleanupMenuHUD(void)
{
	sfSprite_destroy(background);
	background = NULL;
	for (int i = 0; i < 4; i++)
	{
		sfText_destroy(button[i].text);
		button[i].text = NULL;
	}
	for (int i = 0; i < 2; i++)
	{
		sfSprite_destroy(select[i].sprite);
		select[i].sprite = NULL;
	}
	for (int i = 0; i < leafCount; i++)
	{
		DeleteLeaf(i);
		leafCount++;
	}
}

void SetSelection(int _sel)
{
	if (selection + _sel >= 0 && selection + _sel <= 3 && !IsMouseOnButtonMenuHUD())
	{
		selection += _sel;
	}
}

int GetSelection(void)
{
	return selection;
}

sfBool IsMouseOnButtonMenuHUD(void)
{
	sfFloatRect hit = sfText_getGlobalBounds(button[selection].text);
	if (sfFloatRect_contains(&hit, mousePos.x, mousePos.y))
	{
		return sfTrue;
	}
	return sfFalse;
}

void AddLeaf(sfVector2f _position)
{
	Leaf temp = { 0 };
	temp.sprite = sfRectangleShape_create();
	sfRectangleShape_setSize(temp.sprite, (sfVector2f) { 5, 5 });
	sfRectangleShape_setFillColor(temp.sprite, (sfColor) { 20, 47, 48, 255 });
	sfFloatRect hitbox = sfRectangleShape_getGlobalBounds(temp.sprite);
	sfRectangleShape_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfRectangleShape_setPosition(temp.sprite, _position);

	temp.turningLeft = rand() % 2;
	temp.x = 0;
	temp.y = 0;

	leaf[leafCount] = temp;
	leafCount++;
}

void DeleteLeaf(int _ID)
{
	sfRectangleShape_destroy(leaf[_ID].sprite);
	leaf[_ID].sprite = NULL;

	for (int i = _ID; i < leafCount - 1; i++)
	{
		Leaf temp = leaf[i];
		leaf[i] = leaf[i + 1];
		leaf[i + 1] = temp;
	}

	leafCount--;
}
