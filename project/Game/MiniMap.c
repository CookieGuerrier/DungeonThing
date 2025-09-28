#include "MiniMap.h"

MiniMap mini[30];
sfTexture* textureMiniMap[6];
int miniCount;
int current;

void LoadMiniMap(void)
{
	miniCount = 0;
	current = 0;
	textureMiniMap[BATTLE] = sfTexture_createFromFile("Assets/Texture/MiniMap/battle.png", NULL);
	textureMiniMap[BOSS] = sfTexture_createFromFile("Assets/Texture/MiniMap/end.png", NULL);
	textureMiniMap[EXIT] = sfTexture_createFromFile("Assets/Texture/MiniMap/exit.png", NULL);
	textureMiniMap[SHOP] = sfTexture_createFromFile("Assets/Texture/MiniMap/shop.png", NULL);
	textureMiniMap[START] = sfTexture_createFromFile("Assets/Texture/MiniMap/start.png", NULL);
	textureMiniMap[SPECIAL] = sfTexture_createFromFile("Assets/Texture/MiniMap/special.png", NULL);
}

void UpdateMiniMap(sfRenderWindow* _window, float _dt)
{

}

void DrawMiniMap(sfRenderWindow* _window, sfBool _debugMode)
{
	for (int i = 0; i < miniCount; i++)
	{
		if (mini[i].disco || _debugMode)
		{
			sfRenderWindow_drawSprite(_window, mini[i].sprite, NULL);
		}
	}
}

void CleanupMiniMap(void)
{
	for (int i = 0; i < miniCount; i++)
	{
		DeleteMiniMap(i);
		miniCount++;
	}

	for (int i = 0; i < 4; i++)
	{
		sfTexture_destroy(textureMiniMap[i]);
		textureMiniMap[i] = NULL;
	}
}

void AddMiniMap(sfVector2f _position, int _type)
{
	if (miniCount < 30)
	{
		MiniMap temp = { 0 };

		temp.sprite = sfSprite_create(); 
		if (_type <= SPECIAL)
			sfSprite_setTexture(temp.sprite, textureMiniMap[_type], sfTrue);
		else
			sfSprite_setTexture(temp.sprite, textureMiniMap[0], sfTrue);

		sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
		sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		sfSprite_setColor(temp.sprite, (sfColor) { 255, 255, 255, 150 });

		//Pos
		float x = (_position.x - 1500) / SCREEN_WIDTH;
		float y = (_position.y - 1500) / SCREEN_HEIGHT;
		sfSprite_setPosition(temp.sprite, (sfVector2f) { SCREEN_WIDTH / 2 + (x * hitbox.width), SCREEN_HEIGHT / 2 + (y * hitbox.height)});
		
		mini[miniCount] = temp;
		miniCount++;
	}
}

void DeleteMiniMap(int _ID)
{
	sfSprite_destroy(mini[_ID].sprite);
	mini[_ID].sprite = NULL;

	for (int i = _ID; i < miniCount - 1; i++)
	{
		MiniMap temp = mini[i];
		mini[i] = mini[i + 1];
		mini[i + 1] = temp;
	}

	miniCount--;
}

void ClearMiniMap(void)
{
	for (int i = 0; i < miniCount; i++)
	{
		DeleteMiniMap(i);
		miniCount++;
	}
	miniCount = 0;
	current = 0;
}

void PositionMini(int _ID)
{
	sfSprite_setColor(mini[current].sprite, (sfColor) { 255, 255, 255, 150 });
	sfSprite_setColor(mini[_ID].sprite, (sfColor) { 255, 255, 255, 230 });
	mini[_ID].disco = sfTrue;
	current = _ID;
}