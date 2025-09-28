#include "Menu.h"
#include "Music.h"

MenuData menuData;
sfBool toGame;

void LoadMenu(void)
{
	LoadMenuHUD();
	SetOpacityVeil(0, 5);
	PlayMusic(0);
	toGame = sfFalse;
}

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _key)
{
	switch (_key.code)
	{
	case sfKeySpace:
		PlaySound(1);
		switch (GetSelection())
		{
		case 0:
			toGame = sfTrue;
			break;
		case 1:
			sfRenderWindow_close(_renderWindow);
			break;
		}
		break;
	case sfKeyZ:
		SetSelection(-1);
		break;
	case sfKeyS:
		SetSelection(1);
		break;
	case sfKeyUp:
		SetSelection(-1);
		break;
	case sfKeyDown:
		SetSelection(1);
		break;
	default:
		break;
	}
}

void MousePressedMenu(sfRenderWindow* _renderWindow, sfMouseButtonEvent _mouse)
{
	switch (_mouse.button)
	{
	case sfMouseLeft:
	{
		if (IsMouseOnButtonMenuHUD())
		{
			PlaySound(1);
			switch (GetSelection())
			{
			case 0:
				toGame = sfTrue;
				break;
			case 1:
				sfRenderWindow_close(_renderWindow);
				break;
			}
		}
	}
	break;
	}
}

void UpdateMenu(float _dt, sfRenderWindow* _window)
{
	if (toGame)
	{
		SetOpacityVeil(255, 5);

		if (GetOpacity() == 255)
		{
			CleanupMenu();
			LoadGame(_window);
			SetGameState(GAME);
		}
	}
	else
	{
		UpdateMenuHUD(_dt, _window);
	}
}

void DrawMenu(sfRenderWindow* _renderWindow)
{
	SetView(0, _renderWindow);
	DrawMenuHUD(_renderWindow);

	SetView(1, _renderWindow);
	DrawVeil(_renderWindow);
}

void CleanupMenu(void)
{
	CleanupMenuHUD();
}
