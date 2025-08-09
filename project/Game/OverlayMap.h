#ifndef OVERLAYMAP_H
#define OVERLAYMAP_H

#include "Common.h"

typedef struct Overlay
{
	sfSprite* sprite;
}Overlay;

void LoadOverlay(void);
void DrawOverlay(sfRenderWindow* _window, sfBool _debugMode);
void CleanupOverlay(void);

void AddOverlay(sfVector2f _pos, int _type);
void DeleteOverlay(int _ID);

#endif // !OVERLAYMAP_H