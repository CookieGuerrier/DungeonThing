#include "Menu.h"

MenuData menuData;

void LoadMenu(void)
{
	LoadMenuHUD();
}

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _key)
{
	switch (_key.code)
	{
	case sfKeyX:
		//TEMPORAIRE
		sfRenderWindow_close(_renderWindow);
		break;
	case sfKeySpace:
		switch (GetSelection())
		{
		case 0:
			CleanupMenu();
			LoadGame(_renderWindow);
			SetGameState(GAME);
			break;
		case 3:
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
		if (IsMouseOnButton())
		{
			switch (GetSelection())
			{
			case 0:
				CleanupMenu();
				LoadGame(_renderWindow);
				SetGameState(GAME);
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

void UpdateMenu(float _dt, sfRenderWindow* _window)
{
	UpdateMenuHUD(_dt, _window);
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
