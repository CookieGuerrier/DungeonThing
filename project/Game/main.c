#include "Common.h"
#include "GameState.h"
#include "Menu.h"
#include "Game.h"
#include "GameOver.h"

#pragma region Struct
typedef struct MainData
{
	sfRenderWindow* renderWindow;
	sfClock* clock;
}MainData;

#pragma endregion

#pragma region GameLoop
void Load(MainData* const _mainData);
void PollEvent(MainData* const _mainData);
void KeyPressed(sfRenderWindow* const _renderWindow, sfKeyEvent _key);
void MousePressed(sfRenderWindow* const _renderWindow,sfMouseButtonEvent _mouse);
void Update(MainData* const _mainData);
void Draw(MainData* const _mainData);
void Cleanup(MainData* const _mainData);
#pragma endregion

#pragma region Functions
void LoadMainData(MainData* const _mainData);
void CleanupMainData(MainData* const _mainData);
#pragma endregion

int main(void)
{
	MainData mainData = { 0 };

	Load(&mainData);
	while (sfRenderWindow_isOpen(mainData.renderWindow))
	{
		PollEvent(&mainData);
		Update(&mainData);
		Draw(&mainData);
	}
	Cleanup(&mainData);

	return EXIT_SUCCESS;
}

void Load(MainData* const _mainData)
{
	srand(_getpid());

	LoadMainData(_mainData);
	LoadMenu();

	SetGameState(MENU);
}

void PollEvent(MainData* const _mainData)
{
	sfEvent event;
	while (sfRenderWindow_pollEvent(_mainData->renderWindow, &event))
	{
		switch (event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(_mainData->renderWindow);
			break;
		case sfEvtKeyPressed:
			KeyPressed(_mainData->renderWindow, event.key);
			break;
		case sfEvtMouseButtonPressed:
			MousePressed(_mainData->renderWindow, event.mouseButton);
			break;
		default:
			break;
		}
	}
}

void KeyPressed(sfRenderWindow* const _renderWindow, sfKeyEvent _key)
{
	switch (GetGameState())
	{
	case MENU:
		KeyPressedMenu(_renderWindow, _key);
		break;
	case GAME:
		KeyPressedGame(_renderWindow, _key);
		break;
	default:
		break;
	}
}

void MousePressed(sfRenderWindow* const _renderWindow, sfMouseButtonEvent _mouse)
{
	switch (GetGameState())
	{
	case MENU:
		MousePressedMenu(_renderWindow, _mouse);
		break;
	case GAME:
		MousePressedGame(_renderWindow, _mouse);
		break;
	default:
		break;
	}
}

void Update(MainData* const _mainData)
{
	float dt = sfTime_asSeconds(sfClock_restart(_mainData->clock));

	switch (GetGameState())
	{
	case MENU:
		UpdateMenu(dt, _mainData->renderWindow);
		break;
	case GAME:
		UpdateGame(dt, _mainData->renderWindow);
		break;
	default:
		break;
	}
	UpdateCamera(_mainData->renderWindow, dt);
}

void Draw(MainData* const _mainData)
{
	sfRenderWindow_clear(_mainData->renderWindow, sfBlack);

	switch (GetGameState())
	{
	case MENU:
		DrawMenu(_mainData->renderWindow);
		break;
	case GAME:
		DrawGame(_mainData->renderWindow);
		break;
	default:
		break;
	}

	sfRenderWindow_display(_mainData->renderWindow);
}

void Cleanup(MainData* const _mainData)
{
	CleanupMainData(_mainData);

	switch (GetGameState())
	{
	case MENU:
		CleanupMenu();
		break;
	case GAME:
		CleanupGame();
		break;
	default:
		break;
	}
}

void LoadMainData(MainData* const _mainData)
{
	_mainData->clock = sfClock_create();
	sfVideoMode videoMode = { SCREEN_WIDTH, SCREEN_HEIGHT, BPP };
	_mainData->renderWindow = sfRenderWindow_create(videoMode, "Game", sfFullscreen, NULL);
	sfRenderWindow_setFramerateLimit(_mainData->renderWindow, 60);
	sfRenderWindow_setKeyRepeatEnabled(_mainData->renderWindow, sfFalse);

	LoadCamera(_mainData->renderWindow);
}

void CleanupMainData(MainData* const _mainData)
{
	sfClock_destroy(_mainData->clock);
	_mainData->clock = NULL;
	sfRenderWindow_destroy(_mainData->renderWindow);
	_mainData->renderWindow = NULL;

	DestroyCamera();
}