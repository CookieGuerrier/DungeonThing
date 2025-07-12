#include "Map.h"

MapPiece map[30];
sfTexture* mainTexture[MAX];
int mapStart;
sfBool startPlaced;
sfBool shopPlaced;
int mapCount;

int mapCurrent;
int enemyCurrent;

sfBool battle;

void LoadMap(sfRenderWindow* _window)
{
	mapStart = 0;
	startPlaced = sfFalse;
	shopPlaced = sfFalse;

	mapCount = 0;
	mainTexture[R] = sfTexture_createFromFile("Assets/Texture/Map/rText.png", NULL);
	mainTexture[L] = sfTexture_createFromFile("Assets/Texture/Map/lText.png", NULL);
	mainTexture[U] = sfTexture_createFromFile("Assets/Texture/Map/uText.png", NULL);
	mainTexture[D] = sfTexture_createFromFile("Assets/Texture/Map/dText.png", NULL);

	mainTexture[LR] = sfTexture_createFromFile("Assets/Texture/Map/lrText.png", NULL);
	mainTexture[LRU] = sfTexture_createFromFile("Assets/Texture/Map/lruText.png", NULL);
	mainTexture[LRD] = sfTexture_createFromFile("Assets/Texture/Map/lrdText.png", NULL);
	mainTexture[LRUD] = sfTexture_createFromFile("Assets/Texture/Map/lrudText.png", NULL);

	CreateLevel(1);
}

void UpdateMap(float _dt, sfRenderWindow* _window)
{
	if (mapCurrent != GetCurrentMap())
	{
		SetCameraMap(GetCurrentMap(), _window);
		mapCurrent = GetCurrentMap();
		PositionMini(mapCurrent);
		if (map[mapCurrent].element == BATTLE)
		{
			//Battle start
			enemyCurrent = GetEnemyMap(mapCurrent);
			sfFloatRect hitbox = sfSprite_getGlobalBounds(map[mapCurrent].sprite);

			if (GetPlayerPos().x < (hitbox.left + 300))
			{
				PlayerTransition(2);
			}
			else if (GetPlayerPos().x > (hitbox.left + hitbox.width - 300))
			{
				PlayerTransition(3);
			}
			else if (GetPlayerPos().y > (hitbox.top + 300))
			{
				PlayerTransition(1);
			}
			else if (GetPlayerPos().y < (hitbox.top + hitbox.height - 300))
			{
				PlayerTransition(0);
			}


			if (enemyCurrent > 0)
			{
				sfVector2f pos = sfSprite_getPosition(map[mapCurrent].sprite);
				CreateBattleBorder(pos, map[mapCurrent].type);
				battle = sfTrue;
			}
		}
	}
	else
	{
		if (battle)
		{
			if (enemyCurrent <= 0)
			{
				MoveObject(0, (sfVector2f) { 0,0 });
				MoveObject(1, (sfVector2f) { 0,0 });
				MoveObject(2, (sfVector2f) { 0,0 });
				MoveObject(3, (sfVector2f) { 0,0 });
				battle = sfFalse;
			}
		}
	}
}

void DrawMap(sfRenderWindow* _window, sfBool _debugMode)
{
	for (int i = 0; i < mapCount; i++)
	{
		sfRenderWindow_drawSprite(_window, map[i].sprite, NULL);
	}
}

void CleanupMap(void)
{
	for (int i = 0; i < MAX; i++)
	{
		sfTexture_destroy(mainTexture[i]);
		mainTexture[i] = NULL;
	}

	for (int i = 0; i < mapCount; i++)
	{
		DeleteMap(i);
		mapCount++;
	}
}

void AddMap(MapType _type, sfVector2f _pos, MapType _source, ElementType _element)
{
	ElementType element = _element;

	//Safety
	for (int n = 0; n < mapCount; n++)
	{
		sfVector2f posStuff = sfSprite_getPosition(map[n].sprite);
		if (posStuff.x == _pos.x && posStuff.y == _pos.y)
		{
			//Block off
			switch (_source)
			{
			case R:
				AddObject((sfVector2f) { _pos.x + SCREEN_WIDTH + 20, _pos.y + SCREEN_HEIGHT / 2 }, 0, WALL_BLOCKRL);
				break;
			case L:
				AddObject((sfVector2f) { _pos.x - 20, _pos.y + SCREEN_HEIGHT / 2 }, 0, WALL_BLOCKRL);
				break;
			case U:
				AddObject((sfVector2f) { _pos.x + SCREEN_WIDTH / 2, _pos.y + SCREEN_HEIGHT }, 0, WALL_BLOCKUD);
				break;
			case D:
				AddObject((sfVector2f) { _pos.x / 2, _pos.y + SCREEN_HEIGHT }, 0, WALL_BLOCKUD);
				break;
			default:
				break;
			}
			return;
		}
	}

	MapPiece temp = { 0 };
	temp.sprite = sfSprite_create();
	temp.type = _type;

	sfSprite_setTexture(temp.sprite, mainTexture[_type], sfTrue);
	sfSprite_setPosition(temp.sprite, _pos);

	//Start
	if (!startPlaced)
	{
		if (_type == L || _type == R || _type == D || _type == U)
		{
			startPlaced = sfTrue;
			element = START;
		}
	}
	//Shop
	else if (!shopPlaced)
	{
		if (_type == L || _type == R || _type == D || _type == U)
		{
			shopPlaced = sfTrue;
			element = SHOP;
		}
	}

	temp.element = element;

	//Type
	switch (_type)
	{
	case R:
		temp.r = sfTrue;
		break;
	case L:
		temp.l = sfTrue;
		break;
	case U:
		temp.u = sfTrue;
		break;
	case D:
		temp.d = sfTrue;
		break;
	case LR:
		temp.l = sfTrue;
		temp.r = sfTrue;
		break;
	case LRU:
		temp.l = sfTrue;
		temp.r = sfTrue;
		temp.u = sfTrue;
		break;
	case LRD:
		temp.l = sfTrue;
		temp.r = sfTrue;
		temp.d = sfTrue;
		break;
	case LRUD:
		temp.l = sfTrue;
		temp.r = sfTrue;
		temp.u = sfTrue;
		temp.d = sfTrue;
		break;
	default:
		break;
	}
	switch (_source)
	{
	case R:
		temp.r = sfFalse;
		break;
	case L:
		temp.l = sfFalse;
		break;
	case U:
		temp.u = sfFalse;
		break;
	case D:
		temp.d = sfFalse;
		break;
	default:
		break;
	}

	//Walls
	sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
	//Up
	if (_type == D || _type == LRD || _type == LR || _type == L || _type == R)
	{
		AddWall((sfVector2f) { hitbox.left, hitbox.top }, sfFalse, (sfVector2f) { SCREEN_WIDTH, 80 }, sfFalse);
	}
	else
	{
		AddWall((sfVector2f) { hitbox.left, hitbox.top }, sfFalse, (sfVector2f) { SCREEN_WIDTH / 2 - 70, 80 }, sfFalse);
		AddWall((sfVector2f) { hitbox.left + SCREEN_WIDTH / 2 + 70, hitbox.top }, sfFalse, (sfVector2f) { SCREEN_WIDTH / 2 - 70, 80 }, sfFalse);
	}
	//Down
	if (_type == LR || _type == LRU || _type == L || _type == R || _type == U)
	{

		AddWall((sfVector2f) { hitbox.left, hitbox.top + SCREEN_HEIGHT - 40 }, sfFalse, (sfVector2f) { SCREEN_WIDTH, 40 }, sfFalse);
	}
	else
	{
		AddWall((sfVector2f) { hitbox.left, hitbox.top + SCREEN_HEIGHT - 40 }, sfFalse, (sfVector2f) { SCREEN_WIDTH / 2 - 70, 80 }, sfFalse);
		AddWall((sfVector2f) { hitbox.left + SCREEN_WIDTH / 2 + 70, hitbox.top + SCREEN_HEIGHT - 40 }, sfFalse, (sfVector2f) { SCREEN_WIDTH / 2 - 70, 80 }, sfFalse);
	}
	//Right
	if (_type == D || _type == L || _type == U)
	{
		AddWall((sfVector2f) { hitbox.left + SCREEN_WIDTH, hitbox.top }, sfTrue, (sfVector2f) { SCREEN_HEIGHT, 40 }, sfFalse);
	}
	else
	{
		AddWall((sfVector2f) { hitbox.left + SCREEN_WIDTH, hitbox.top + SCREEN_HEIGHT / 2 + 70 }, sfTrue, (sfVector2f) { SCREEN_HEIGHT / 2 - 70, 40 }, sfFalse);
		AddWall((sfVector2f) { hitbox.left + SCREEN_WIDTH, hitbox.top }, sfTrue, (sfVector2f) { SCREEN_HEIGHT / 2 - 70, 40 }, sfFalse);
	}
	//Left
	if (_type == D || _type == R || _type == U)
	{
		AddWall((sfVector2f) { hitbox.left + 40, hitbox.top }, sfTrue, (sfVector2f) { SCREEN_HEIGHT, 40 }, sfFalse);
	}
	else
	{
		AddWall((sfVector2f) { hitbox.left + 40, hitbox.top }, sfTrue, (sfVector2f) { SCREEN_HEIGHT / 2 - 70, 40 }, sfFalse);
		AddWall((sfVector2f) { hitbox.left + 40, hitbox.top + SCREEN_HEIGHT / 2 + 70 }, sfTrue, (sfVector2f) { SCREEN_HEIGHT / 2 - 70, 40 }, sfFalse);
	}

	//Object
	int prob = 20;
	//Left
	for (size_t i = 3; i < 19; i++)
	{
		int randBull = rand() % prob;
		if (i != 9 && i != 10 && i != 11 && randBull == 0)
		{
			AddObject((sfVector2f) { hitbox.left + 70, hitbox.top + (60 * i) - 60 }, 0, POT);
		}
	}
	//Right
	for (size_t i = 3; i < 19; i++)
	{
		int randBull = rand() % prob;
		if (i != 9 && i != 10 && i != 11 && randBull == 0)
		{
			AddObject((sfVector2f) { hitbox.left + hitbox.width - 70, hitbox.top + (60 * i) - 60 }, 0, POT);
		}
	}
	//Up
	for (size_t i = 2; i < 30; i++)
	{
		int randBull = rand() % prob;
		if (i != 15 && i != 16 && i != 17 && randBull == 0)
		{
			AddObject((sfVector2f) { hitbox.left + (60 * i) + 10, hitbox.top + 120 }, 0, POT);
		}
	}
	for (size_t i = 2; i < 30; i++)
	{
		int randBull = rand() % prob;
		if (i != 15 && i != 16 && i != 17 && randBull == 0)
		{
			AddObject((sfVector2f) { hitbox.left + (60 * i) + 10, hitbox.top + hitbox.height - 60 }, 0, POT);
		}
	}
	AddMiniMap(_pos, temp.element);

	//Type of stuff
	int ran;
	switch (temp.element)
	{
	case BATTLE:
		ran = rand() % MAX_LEVEL;
		CreateBattle(ran, hitbox);
		break;
	case START:
		PositionMini(mapCount);
		mapStart = mapCount;
		break;
	case SHOP:
		AddObject((sfVector2f) { _pos.x + hitbox.width / 2, _pos.y + hitbox.height / 2 - 150 }, 0, STATUE);
		ran = rand() % 2;
		AddItem(0, ran, (sfVector2f) { _pos.x + hitbox.width / 2 - 250, _pos.y + hitbox.height / 2 + 20});
		ran = rand() % 2;
		AddItem(1, ran, (sfVector2f) { _pos.x + hitbox.width / 2, _pos.y + hitbox.height / 2 + 20});
		ran = rand() % 2;
		AddItem(2, ran, (sfVector2f) { _pos.x + hitbox.width / 2 + 250, _pos.y + hitbox.height / 2 + 20});
		break;
	default:
		break;
	}


	map[mapCount] = temp;
	mapCount++;
}

void DeleteMap(int _ID)
{
	sfSprite_destroy(map[_ID].sprite);
	map[_ID].sprite = NULL;

	for (int i = _ID; i < mapCount; i++)
	{
		MapPiece temp = map[i];
		map[i] = map[i + 1];
		map[i + 1] = temp;
	}

	mapCount--;
}

void CreateLevel(int _pathLength)
{
	//First
	int randPiece = rand() % (LRUD + 1 - 4) + 4;
	AddMap(randPiece, (sfVector2f) { 1500, 1500 }, MAX, BATTLE);

	//Middle
	for (int i = 0; i < _pathLength; i++)
	{
		sfFloatRect pos = sfSprite_getGlobalBounds(map[i].sprite);
		if (map[i].r)
		{
			MapType tab[] = { L, LR, LRU, LRD, LRUD };
			int tabCount = 5;
			randPiece = rand() % tabCount;
			AddMap(tab[randPiece], (sfVector2f) { pos.left + pos.width, pos.top }, L, BATTLE);
			map[i].r = sfFalse;
		}
		if (map[i].l)
		{
			MapType tab[] = { R, LR, LRU, LRD, LRUD };
			int tabCount = 5;
			randPiece = rand() % tabCount;
			AddMap(tab[randPiece], (sfVector2f) { pos.left - pos.width, pos.top }, R, BATTLE);
			map[i].l = sfFalse;
		}
		if (map[i].u)
		{
			MapType tab[] = { D, LRD, LRUD };
			int tabCount = 3;
			randPiece = rand() % tabCount;
			AddMap(tab[randPiece], (sfVector2f) { pos.left, pos.top - pos.height }, D, BATTLE);
			map[i].u = sfFalse;
		}
		if (map[i].d)
		{
			MapType tab[] = { U, LRU, LRUD };
			int tabCount = 3;
			randPiece = rand() % tabCount;
			AddMap(tab[randPiece], (sfVector2f) { pos.left, pos.top + pos.height }, U, BATTLE);
			map[i].d = sfFalse;
		}
	}

	//Ends
	for (int i = 0; i < mapCount; i++)
	{
		sfFloatRect hitboxMap = sfSprite_getGlobalBounds(map[i].sprite);
		ElementType element = BATTLE;
		if (map[i].r)
		{
			MapType piece = L;
			int tabCount = 5;
			AddMap(piece, (sfVector2f) { hitboxMap.left + hitboxMap.width, hitboxMap.top }, L, element);
			map[i].r = sfFalse;
		}
		if (map[i].l)
		{
			MapType piece = R;
			int tabCount = 5;
			AddMap(piece, (sfVector2f) { hitboxMap.left - hitboxMap.width, hitboxMap.top }, R, element);
			map[i].l = sfFalse;
		}
		if (map[i].u)
		{
			MapType piece = D;
			int tabCount = 3;
			AddMap(piece, (sfVector2f) { hitboxMap.left, hitboxMap.top - hitboxMap.height }, D, element);
			map[i].u = sfFalse;
		}
		if (map[i].d)
		{
			MapType piece = U;
			int tabCount = 3;
			AddMap(piece, (sfVector2f) { hitboxMap.left, hitboxMap.top + hitboxMap.height }, U, element);
			map[i].d = sfFalse;
		}
	}
}

void CreateBattle(BattleType _type, sfFloatRect _hitbox)
{
	int ran = rand() % 3;
	switch (_type)
	{
	case ONE:
		ran = rand() % 3;
		AddEnemy(ran, (sfVector2f) { _hitbox.left + _hitbox.width / 2 - 800, _hitbox.top + _hitbox.height / 2 - 300 }, mapCount);
		ran = rand() % 3;
		AddEnemy(ran, (sfVector2f) { _hitbox.left + _hitbox.width / 2 + 750, _hitbox.top + _hitbox.height / 2 + 400 }, mapCount);
		break;
	case TWO:
		ran = rand() % 3;
		AddEnemy(ran, (sfVector2f) { _hitbox.left + _hitbox.width / 2 + 200, _hitbox.top + _hitbox.height / 2 }, mapCount);
		ran = rand() % 3;
		AddEnemy(ran, (sfVector2f) { _hitbox.left + _hitbox.width / 2 - 200, _hitbox.top + _hitbox.height / 2 }, mapCount);
		break;
	default:
		break;
	}
}

void RemoveEnemyCurrent(void)
{
	enemyCurrent--;
}

void CreateBattleBorder(sfVector2f _pos, MapType _type)
{
	if (_type == L || _type == LR || _type == LRU || _type == LRUD || _type == LRD)
	{
		MoveObject(1, (sfVector2f) { _pos.x + 20, _pos.y + SCREEN_HEIGHT / 2 });
	}
	if (_type == R || _type == LR || _type == LRU || _type == LRUD || _type == LRD)
	{
		MoveObject(0, (sfVector2f) { _pos.x + SCREEN_WIDTH - 20, _pos.y + SCREEN_HEIGHT / 2 });
	}
	if (_type == D || _type == LRD || _type == LRUD)
	{
		MoveObject(2, (sfVector2f) { _pos.x + SCREEN_WIDTH / 2, _pos.y + SCREEN_HEIGHT });
	}
	if (_type == U || _type == LRU || _type == LRUD)
	{
		MoveObject(3, (sfVector2f) { _pos.x + SCREEN_WIDTH / 2, _pos.y + 10 });
	}
}

sfVector2f GetSpawnPoint(void)
{
	sfFloatRect hitbox = sfSprite_getGlobalBounds(map[mapStart].sprite);
	return (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 };
}

void SetCameraMap(int _ID, sfRenderWindow* _window)
{
	sfFloatRect hitbox = sfSprite_getGlobalBounds(map[_ID].sprite);
	MoveCamera((sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 }, 8);
}

int GetCurrentMap(void)
{
	for (int i = 0; i < mapCount; i++)
	{
		sfFloatRect pHitbox = GetPlayerHitbox();
		sfFloatRect hitbox = sfSprite_getGlobalBounds(map[i].sprite);
		if (sfFloatRect_intersects(&pHitbox, &hitbox, NULL))
		{
			return i;
		}
	}
	return mapStart;
}

int GetBulletMap(sfFloatRect _hitbox)
{
	for (int i = 0; i < mapCount; i++)
	{
		sfFloatRect pHitbox = _hitbox;
		sfFloatRect hitbox = sfSprite_getGlobalBounds(map[i].sprite);
		if (sfFloatRect_intersects(&pHitbox, &hitbox, NULL))
		{
			return i;
		}
	}
	return mapStart;
}
