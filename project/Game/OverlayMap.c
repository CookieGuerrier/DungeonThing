#include "OverlayMap.h"

sfTexture* textureOverlay[4];
Overlay overlay[30];
int overlayCount;

void LoadOverlay(void)
{
	overlayCount = 0;
	textureOverlay[0] = sfTexture_createFromFile("Assets/Texture/Map/specialArtifact.png",NULL);
	textureOverlay[1] = sfTexture_createFromFile("Assets/Texture/Map/tutoStart.png",NULL);
	textureOverlay[2] = sfTexture_createFromFile("Assets/Texture/Map/nakromOverlay.png",NULL);
	textureOverlay[3] = sfTexture_createFromFile("Assets/Texture/Map/guardOverlay.png",NULL);
}

void DrawOverlay(sfRenderWindow* _window, sfBool _debugMode)
{
	for (int i = 0; i < overlayCount; i++)
	{
		sfRenderWindow_drawSprite(_window, overlay[i].sprite, NULL);
	}
}

void CleanupOverlay(void)
{
	for (int i = 0; i < overlayCount; i++)
	{
		DeleteOverlay(i);
		overlayCount++;
	}
	for (int i = 0; i < 4; i++)
	{
		sfTexture_destroy(textureOverlay[i]);
		textureOverlay[i] = NULL;
	}
}

void AddOverlay(sfVector2f _pos, int _type)
{
	if (overlayCount < 30)
	{
		Overlay temp = { 0 };
		temp.sprite = sfSprite_create();
		sfSprite_setTexture(temp.sprite, textureOverlay[_type], sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
		sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
		sfSprite_setPosition(temp.sprite, _pos);
		overlay[overlayCount] = temp;
		overlayCount++;
	}
}

void DeleteOverlay(int _ID)
{
	sfSprite_destroy(overlay[_ID].sprite);
	overlay[_ID].sprite = NULL;

	for (int i = _ID; i < overlayCount - 1; i++)
	{
		Overlay temp = overlay[i];
		overlay[i] = overlay[i + 1];
		overlay[i + 1] = temp;
	}
		
	overlayCount--;
}

void ClearOverlay(void)
{
	for (int i = 0; i < overlayCount; i++)
	{
		DeleteOverlay(i);
		overlayCount++;
	}
	overlayCount = 0;
}