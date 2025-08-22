#include "Game.h"

sfBool debugMode;
sfBool pauseState;
sfBool gameOverState;

sfVector2f mousePosGame;
sfFont* font;
Selection select[2];
Buttons button[4];
sfTexture* selectionText;
int selection;

sfBool toMenu;

void LoadGame(sfRenderWindow* _window)
{
	font = sfFont_createFromFile("Assets/Font/font.ttf");
	pauseState = sfFalse;
	gameOverState = sfFalse;
	toMenu = sfFalse;
	selection = 0;
	SetOpacityVeil(0, 5);
	LoadWall();
	LoadObject();
	LoadMoney();

	LoadEnemy();
	LoadBoss();
	LoadShop();
	LoadMiniMap();
	LoadOverlay();
	LoadMap(_window);

	LoadPlayer();

	LoadBullet();

	LoadGameHUD(font);

	LoadPauseMenu();
}

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _key)
{
	switch (_key.code)

	case sfKeyEscape:
	{
		if (!gameOverState)
		{
			if (!pauseState)
			{
				pauseState = sfTrue;
				SetOpacityVeil(100, 10);
			}
			else
			{
				pauseState = sfFalse;
				SetOpacityVeil(0, 10);
			}
		}
		break;
	case sfKeySpace:
		if (pauseState || gameOverState)
		{
			switch (GetSelection())
			{
			case 2:
				toMenu = sfTrue;
				break;
			case 3:
				sfRenderWindow_close(_renderWindow);
				break;
			}
		}
		else
		{
			PlayerRoll();
		}
		break;
	case sfKeyZ:
		if (gameOverState)
			SetSelectionGameOver(-1);
		else
			SetSelection(-1);
		break;
	case sfKeyS:
		if (gameOverState)
			SetSelectionGameOver(1);
		else
			SetSelection(1);
		break;
	case sfKeyP:
		if (debugMode)
		{
			debugMode = sfFalse;
		}
		else
		{
			debugMode = sfTrue;
		}
		break;
	default:
		break;
	}
}

void MousePressedGame(sfRenderWindow* _renderWindow, sfMouseButtonEvent _mouse)
{
	switch (_mouse.button)
	{
	case sfMouseLeft:
	{
		if (IsMouseOnButtonGame())
		{
			switch (GetSelection())
			{
			case 2:
				toMenu = sfTrue;
				break;
			case 3:
				sfRenderWindow_close(_renderWindow);
				break;
			}
		}
	}
	break;
	}
}

void UpdateGame(float _dt, sfRenderWindow* _window)
{
	if (toMenu)
	{
		SetOpacityVeil(255, 5);

		if (GetOpacity() == 255)
		{
			CleanupGame();
			LoadMenu();
			SetGameState(MENU);
		}
	}
	else
	{
		if (!pauseState)
		{
			if (!gameOverState)
			{
				if (GetHP() > 0)
				{
					UpdateWall(_dt, _window);
					UpdateObject(_dt, _window);
					UpdateMap(_dt, _window);
					UpdateMoney(_dt, _window);
					UpdateShop(_dt, _window);

					UpdateBullet(_dt, _window);
					UpdateEnemy(_dt, _window);
					UpdateMiniMap(_window, _dt);

					UpdateBoss(_dt, _window);

					UpdateGameHUD(_dt);
				}
			}
			else
			{
				IsMouseOnButtonGame();
				UpdateGameOverMenu(_dt, _window);
			}
			UpdatePlayer(_dt, _window);
		}
		else
		{
			UpdatePauseMenu(_dt, _window);
		}
	}

	if (GetHP() <= 0 && !gameOverState && DeathFinished())
	{
		SetOpacityVeil(100, 10);
		gameOverState = sfTrue;
		selection = 2;
	}
}

void DrawGame(sfRenderWindow* _window)
{
	SetView(0, _window);
	DrawMap(_window, debugMode);
	DrawOverlay(_window, debugMode);
	DrawObject(_window, debugMode);
	DrawWall(_window, debugMode);
	DrawShop(_window, debugMode);

	DrawRope(_window);
	DrawEnemy(_window, debugMode);
	DrawBoss(_window, debugMode);
	DrawMarker(_window);
	DrawMoney(_window, debugMode);
	DrawBullet(_window, debugMode);
	DrawPlayer(_window, debugMode);

	SetView(1, _window);
	DrawGameHUD(_window, debugMode);

	SetView(2, _window);
	DrawMiniMap(_window, debugMode);

	SetView(1, _window);
	DrawVeil(_window);
	if (pauseState)
	{
		DrawPauseMenu(_window);
	}
	else if (gameOverState)
	{
		DrawGameOverMenu(_window);
	}
}

void CleanupGame(void)
{
	CleanupWall();
	CleanupObject();
	CleanupOverlay();
	CleanupMap();
	CleanupMoney();
	CleanupShop();

	CleanupBullet();
	CleanupPlayer();
	CleanupEnemy();
	CleanupBoss();
	CleanupMiniMap();

	CleanupGameHUD(font);
	CleanupPauseMenu();

	sfFont_destroy(font);
	font = NULL;
}

void LoadPauseMenu(void)
{
	selectionText = sfTexture_createFromFile("Assets/Texture/HUD/hudMarker.png", NULL);

	for (int i = 0; i < 4; i++)
	{
		button[i].text = sfText_create();
		sfText_setFont(button[i].text, font);
		sfText_setOutlineColor(button[i].text, sfBlack);
		sfText_setOutlineThickness(button[i].text, 2);
		sfText_setCharacterSize(button[i].text, 70);
	}
	sfText_setString(button[0].text, "Save");
	sfFloatRect hitbox = sfText_getGlobalBounds(button[0].text);
	sfText_setOrigin(button[0].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[0].text, (sfVector2f) { SCREEN_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 - 200 });
	sfText_setColor(button[0].text, (sfColor) { 255, 255, 255, 120 });

	sfText_setString(button[1].text, "Options");
	hitbox = sfText_getGlobalBounds(button[1].text);
	sfText_setOrigin(button[1].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[1].text, (sfVector2f) { SCREEN_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 - 100 });
	sfText_setColor(button[1].text, (sfColor) { 255, 255, 255, 120 });

	sfText_setString(button[2].text, "Menu");
	hitbox = sfText_getGlobalBounds(button[2].text);
	sfText_setOrigin(button[2].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[2].text, (sfVector2f) { SCREEN_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 });

	sfText_setString(button[3].text, "Quit");
	hitbox = sfText_getGlobalBounds(button[3].text);
	sfText_setOrigin(button[3].text, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfText_setPosition(button[3].text, (sfVector2f) { SCREEN_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 + 100 });

	for (int i = 0; i < 2; i++)
	{
		select[i].sprite = sfSprite_create();
		sfSprite_setTexture(select[i].sprite, selectionText, sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(select[i].sprite);
		sfSprite_setOrigin(select[i].sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	}
	sfSprite_setScale(select[0].sprite, (sfVector2f) { -1, 1 });
}

void UpdatePauseMenu(float _dt, sfRenderWindow* _window)
{
	sfVector2f pos = sfText_getPosition(button[selection].text);
	sfFloatRect hitbox = sfText_getGlobalBounds(button[selection].text);
	sfSprite_setPosition(select[0].sprite, (sfVector2f) { pos.x - hitbox.width / 2 - 20, pos.y + 40 });
	sfSprite_setPosition(select[1].sprite, (sfVector2f) { pos.x + hitbox.width / 2 + 20, pos.y + 40 });

	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	mousePosGame = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetHUDView());

	for (int i = 0; i < 4; i++)
	{
		sfFloatRect hit = sfText_getGlobalBounds(button[i].text);
		if (sfFloatRect_contains(&hit, mousePosGame.x, mousePosGame.y))
		{
			selection = i;
		}
	}
}

void DrawPauseMenu(sfRenderWindow* _window)
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

void CleanupPauseMenu(void)
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

void UpdateGameOverMenu(float _dt, sfRenderWindow* _window)
{
	sfVector2f pos = sfText_getPosition(button[selection].text);
	sfFloatRect hitbox = sfText_getGlobalBounds(button[selection].text);
	sfSprite_setPosition(select[0].sprite, (sfVector2f) { pos.x - hitbox.width / 2 - 20, pos.y + 40 });
	sfSprite_setPosition(select[1].sprite, (sfVector2f) { pos.x + hitbox.width / 2 + 20, pos.y + 40 });

	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	mousePosGame = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetHUDView());
	for (int i = 2; i < 4; i++)
	{
		sfFloatRect hit = sfText_getGlobalBounds(button[i].text);
		if (sfFloatRect_contains(&hit, mousePosGame.x , mousePosGame.y ))
		{
			selection = i;
		}
	}
}

void DrawGameOverMenu(sfRenderWindow* _window)
{
	for (int i = 2; i < 4; i++)
	{
		sfRenderWindow_drawText(_window, button[i].text, NULL);
	}
	for (int i = 0; i < 2; i++)
	{
		sfRenderWindow_drawSprite(_window, select[i].sprite, NULL);
	}
}

void SetSelectionGameOver(int _sel)
{
	if (selection + _sel >= 2 && selection + _sel <= 3 && !IsMouseOnButtonGame())
	{
		selection += _sel;
	}
}


sfBool IsMouseOnButtonGame(void)
{
	sfFloatRect hit = sfText_getGlobalBounds(button[selection].text);
	if (sfFloatRect_contains(&hit, mousePosGame.x, mousePosGame.y))
	{
		return sfTrue;	
	}
	return sfFalse;
}