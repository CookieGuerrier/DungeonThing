#include "Menu.h"

MenuData menuData;

void LoadMenu(void)
{
}

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent _key)
{
	switch (_key.code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(_renderWindow);
		break;
	case sfKeySpace:
		CleanupMenu();
		LoadGame(_renderWindow);
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{
}

void DrawMenu(sfRenderWindow* _renderWindow)
{
}

void CleanupMenu(void)
{
}
