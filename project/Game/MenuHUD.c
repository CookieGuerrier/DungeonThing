#include "MenuHUD.h"

int selection;
sfFont* font;
Buttons button[4];
sfTexture* selectionText;
Selection select[2];

void LoadMenuHUD(void)
{
	font = sfFont_createFromFile("Assets/Font/font.ttf");
	selectionText = sfTexture_createFromFile("Assets/Texture/HUD/hudMarker.png", NULL);

	for (int i = 0; i < 4; i++)
	{
		button[i].text = sfText_create();
		sfText_setFont(button[i].text, font);
		sfText_setOutlineColor(button[i].text, sfBlack);
		sfText_setOutlineThickness(button[i].text, 2);
		sfText_setCharacterSize(button[i].text, 70);
	}
	SetCamera((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
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
	sfVector2f pos = sfText_getPosition(button[selection].text);
	sfFloatRect hitbox = sfText_getGlobalBounds(button[selection].text);
	sfSprite_setPosition(select[0].sprite, (sfVector2f) { pos.x - hitbox.width / 2 - 20, pos.y + 40 });
	sfSprite_setPosition(select[1].sprite, (sfVector2f) { pos.x + hitbox.width / 2 + 20, pos.y + 40 });

	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	sfVector2f mousePos = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetView());
	for (int i = 0; i < 4; i++)
	{
		sfFloatRect hit = sfText_getGlobalBounds(button[i].text);
		if (sfFloatRect_contains(&hit, mousePos.x, mousePos.y))
		{
			selection = i;
		}
	}
}

void DrawMenuHUD(sfRenderWindow* _window)
{
	for (int i = 0; i < 4; i++)
	{
		sfRenderWindow_drawText(_window, button[i].text, NULL);
	}
	for (int i = 0; i < 2; i++)
	{
		sfRenderWindow_drawSprite(_window, select[i].sprite, NULL);
	}
}

void CleanupMenuHUD(void)
{
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
}

void SetSelection(int _sel)
{
	if (selection + _sel >= 0 && selection + _sel <= 3)
	{
		selection += _sel;
	}
}

int GetSelection(void)
{
	return selection;
}