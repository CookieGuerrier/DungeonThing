#include "Enemy.h"

Enemy enemy[100];
sfTexture* enemyTexture[8];
sfTexture* textureShadow;
int enemyCount;

sfFont* font;
Marker marker[50];
int markerCount;

int enemyRoped;
float closestDis;

void LoadEnemy(void)
{
	int enemyRoped = 999;
	enemyCount = 0;
	markerCount = 0;
	enemyTexture[REA_BASE] = sfTexture_createFromFile("Assets/Texture/Enemy/rea_base.png", NULL);
	enemyTexture[REA_SHOTGUN] = sfTexture_createFromFile("Assets/Texture/Enemy/rea_shotgun.png", NULL);
	enemyTexture[REA_CLOTH] = sfTexture_createFromFile("Assets/Texture/Enemy/rea_cloth.png", NULL);
	enemyTexture[SLIME] = sfTexture_createFromFile("Assets/Texture/Enemy/slime.png", NULL);
	enemyTexture[BAT] = sfTexture_createFromFile("Assets/Texture/Enemy/bat.png", NULL);
	enemyTexture[TORMENTED_SOUL] = sfTexture_createFromFile("Assets/Texture/Enemy/tormented_soul.png", NULL);
	enemyTexture[BIG_CRAB] = sfTexture_createFromFile("Assets/Texture/Enemy/big_crab.png", NULL);
	enemyTexture[TINY_CRAB] = sfTexture_createFromFile("Assets/Texture/Enemy/tiny_crab.png", NULL);
	textureShadow = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);

	font = sfFont_createFromFile("Assets/Font/font.ttf");
}

void LoadEnemyAnimation(Enemy* _enemy)
{
	_enemy->anims = calloc(3, sizeof(Anim*));
	if (!_enemy->anims) return;
	switch (_enemy->type)
	{
	case REA_BASE: {
		sfVector2u size = sfTexture_getSize(enemyTexture[REA_BASE]);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 4 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[REA_BASE], first, 6, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 75 * 2, size.x / 8, size.y / 4 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[REA_BASE], first, 8, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0,  75 * 3, size.x / 8, size.y / 4 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[REA_BASE], first, 4, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
		break;
	}
	case REA_SHOTGUN: {
		sfVector2u size = sfTexture_getSize(enemyTexture[REA_SHOTGUN]);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 4 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[REA_SHOTGUN], first, 6, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 75 * 2, size.x / 8, size.y / 4 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[REA_SHOTGUN], first, 8, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0,  75 * 3, size.x / 8, size.y / 4 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[REA_SHOTGUN], first, 4, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
	case REA_CLOTH: {
		sfVector2u size = sfTexture_getSize(enemyTexture[REA_CLOTH]);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 4 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[REA_CLOTH], first, 6, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 75 * 2, size.x / 8, size.y / 4 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[REA_CLOTH], first, 8, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0,  75 * 3, size.x / 8, size.y / 4 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[REA_CLOTH], first, 4, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
	case SLIME: {
		sfVector2u size = sfTexture_getSize(enemyTexture[SLIME]);
		sfIntRect first = { 0, 0, size.x / 4, size.y / 4 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[SLIME], first, 4, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 8.f, (float)size.y / 12.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 60 * 2, size.x / 4, size.y / 4 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[SLIME], first, 4, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 8.f, (float)size.y / 12.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0,  60 * 3, size.x / 4, size.y / 4 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[SLIME], first, 3, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 8.f, (float)size.y / 12.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
	case BAT: {
		sfVector2u size = sfTexture_getSize(enemyTexture[BAT]);
		sfIntRect first = { 0, 0, size.x / 5, size.y / 4 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[BAT], first, 4, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 10.f, (float)size.y / 12.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 60 * 2, size.x / 5, size.y / 4 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[BAT], first, 4, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 10.f, (float)size.y / 12.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0,  60 * 3, size.x / 5, size.y / 4 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[BAT], first, 5, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 10.f, (float)size.y / 12.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
	case TORMENTED_SOUL: {
		sfVector2u size = sfTexture_getSize(enemyTexture[TORMENTED_SOUL]);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 2 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[TORMENTED_SOUL], first, 8, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 0, size.x / 8, size.y / 2 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[TORMENTED_SOUL], first, 8, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 85, size.x / 8, size.y / 2 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[TORMENTED_SOUL], first, 8, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
	case BIG_CRAB: {
		sfVector2u size = sfTexture_getSize(enemyTexture[BIG_CRAB]);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 4 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[BIG_CRAB], first, 4, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 95 * 2, size.x / 8, size.y / 4 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[BIG_CRAB], first, 8, 1 / 6.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 95 * 3, size.x / 8, size.y / 4 };
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[BIG_CRAB], first, 3, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
	case TINY_CRAB: {
		sfVector2u size = sfTexture_getSize(enemyTexture[TINY_CRAB]);
		sfIntRect first = { 0, 0, size.x / 4, size.y / 2 };
		_enemy->anims[IDLE_E] = CreateAnim(enemyTexture[TINY_CRAB], first, 4, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 8.f, (float)size.y / 2.f }, sfTrue);
		_enemy->anims[IDLE_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[IDLE_E]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 0, size.x / 4, size.y / 2 };
		_enemy->anims[WALK_E] = CreateAnim(enemyTexture[TINY_CRAB], first, 4, 1 / 8.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 8.f, (float)size.y / 2.f }, sfTrue);
		_enemy->anims[WALK_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[WALK_E]->events = malloc(sizeof(AnimEvent));
	}break;
	default:
		break;
	}
}

void UpdateEnemy(float _dt, sfRenderWindow* _window)
{

	for (int i = 0; i < enemyCount; i++)
	{
		if (enemy[i].life > 0)
		{
			//Collider
			sfVector2f pos = sfSprite_getPosition(enemy[i].sprite);
			switch (enemy[i].type)
			{
			case REA_BASE:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 2 });
				break;
			case REA_SHOTGUN:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 2 });
				break;
			case REA_CLOTH:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 2 });
				break;
			case SLIME:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 20 });
				break;
			case BAT:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 20 });
				break;
			case TORMENTED_SOUL:
				if (GetEnemyMap(GetCurrentMap()) == 1 && GetCurrentMap() == enemy[i].id)
				{
					enemy[i].life--;
				}
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 20 });
				break;
			case BIG_CRAB:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y });
				break;
			case TINY_CRAB:
				sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y });
				break;
			}

			//Function
			if (GetBattleDelay() < 0)
			{
				if (GetCurrentMap() == enemy[i].id)
				{
					EnemyMove(i, _dt);
					EnemyShoot(i, _dt);
				}

				//Animations
				if (enemy[i].hurtFrame > 0)
				{
					enemy[i].hurtFrame -= _dt;
				}
				else
				{
					if (enemy[i].velocity.x == 0 && enemy[i].velocity.y == 0)
					{
						UpdateAnim(_dt, enemy[i].anims[IDLE_E]);
					}
					else
					{
						UpdateAnim(_dt, enemy[i].anims[WALK_E]);
					}
				}
			}
			sfSprite_setPosition(enemy[i].spriteShadow, (sfVector2f) { pos.x + 2, pos.y + 2 });
		}
		else
		{
			EnemyDeath(i, _dt);
		}
	}

	//Marker
	for (int i = 0; i < markerCount; i++)
	{
		sfText_move(marker[i].text, (sfVector2f) { 0, -2 });
		marker[i].alpha -= 10;
		sfText_setFillColor(marker[i].text, (sfColor) { 255, 255 - (50 * (marker[i].dmg - 1)), 255 - (50 * (marker[i].dmg - 1)), marker[i].alpha });
		sfText_setOutlineColor(marker[i].text, (sfColor) { 0, 0, 0, marker[i].alpha });
		if (marker[i].alpha <= 0)
		{
			DeleteMarker(i);
		}
	}
}

void DrawEnemy(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (enemy[i].type != TORMENTED_SOUL && enemy[i].type != TINY_CRAB)
		{
			sfRenderWindow_drawSprite(_window, enemy[i].spriteShadow, NULL);
		}
		sfRenderWindow_drawSprite(_window, enemy[i].sprite, NULL);
		if (_debug)
		{
			sfRenderWindow_drawRectangleShape(_window, enemy[i].collider, NULL);
		}
	}
	for (int i = 0; i < markerCount; i++)
	{
		sfRenderWindow_drawText(_window, marker[i].text, NULL);
	}
}

void CleanupEnemy(void)
{
	for (int i = 0; i < enemyCount; i++)
	{
		DeleteEnemy(i);
		enemyCount++;
	}
	sfTexture_destroy(enemyTexture[REA_BASE]);
	enemyTexture[REA_BASE] = NULL;
	sfTexture_destroy(enemyTexture[REA_SHOTGUN]);
	enemyTexture[REA_SHOTGUN] = NULL;
	sfTexture_destroy(enemyTexture[REA_CLOTH]);
	enemyTexture[REA_CLOTH] = NULL;
	sfTexture_destroy(enemyTexture[SLIME]);
	enemyTexture[SLIME] = NULL;
	sfTexture_destroy(enemyTexture[BAT]);
	enemyTexture[BAT] = NULL;
	sfTexture_destroy(enemyTexture[TORMENTED_SOUL]);
	enemyTexture[TORMENTED_SOUL] = NULL;
	sfTexture_destroy(enemyTexture[BIG_CRAB]);
	enemyTexture[BIG_CRAB] = NULL;
	sfTexture_destroy(enemyTexture[TINY_CRAB]);
	enemyTexture[TINY_CRAB] = NULL;

	sfFont_destroy(font);
	font = NULL;
	for (int i = 0; i < markerCount; i++)
	{
		DeleteMarker(i);
		markerCount++;
	}
}

void AddEnemy(TypeEnemy _type, sfVector2f _pos, int _idMap)
{
	if (enemyCount < 100)
	{
		Enemy temp = { 0 };

		//Sprite
		temp.sprite = sfSprite_create();
		sfSprite_setTexture(temp.sprite, enemyTexture[_type], sfTrue);
		sfSprite_setPosition(temp.sprite, _pos);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
		sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		temp.collider = sfRectangleShape_create();
		temp.color = 255;
		temp.misc = 1;
		float ran = 0;
		//Type
		switch (_type)
		{
		case REA_BASE:
			temp.life = 7;
			temp.speed = 150;
			ran = (float)(rand() % 11 - 5);
			temp.fireRate = (ran / 10);
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 40, 60 });
			break;
		case REA_SHOTGUN:
			temp.life = 7;
			temp.speed = 150;
			ran = (float)(rand() % 11 - 5);
			temp.fireRate = (ran / 10);
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 40, 60 });
			break;
		case REA_CLOTH:
			temp.life = 7;
			temp.speed = 150;
			ran = (float)(rand() % 11 - 5);
			temp.fireRate = (ran / 10);
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 40, 60 });
			break;
		case SLIME:
			temp.life = 5;
			temp.speed = 900;
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 40, 40 });
			break;
		case BAT:
			temp.life = 5;
			temp.speed = 700;
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 80, 60 });
			break;
		case TORMENTED_SOUL:
			temp.life = 1;
			temp.speed = 250;
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 60, 85 });
			break;
		case BIG_CRAB:
			temp.life = 15;
			temp.speed = 80;
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 180, 75 });
			break;
		case TINY_CRAB:
			temp.life = 2;
			temp.speed = 150;
			sfRectangleShape_setSize(temp.collider, (sfVector2f) { 45, 20 });
			break;
		default:
			break;
		}
		sfRectangleShape_setOutlineThickness(temp.collider, 3);
		sfRectangleShape_setOutlineColor(temp.collider, sfRed);
		sfRectangleShape_setFillColor(temp.collider, (sfColor) { 0, 0, 0, 0 });
		hitbox = sfRectangleShape_getGlobalBounds(temp.collider);
		sfRectangleShape_setOrigin(temp.collider, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		//Shadow
		temp.spriteShadow = sfSprite_create();
		sfSprite_setTexture(temp.spriteShadow, textureShadow, sfTrue);
		hitbox = sfSprite_getGlobalBounds(temp.spriteShadow);
		sfSprite_setOrigin(temp.spriteShadow, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		temp.velocity = (sfVector2f){ 0 };
		temp.id = _idMap;
		temp.type = _type;

		LoadEnemyAnimation(&temp);

		enemy[enemyCount] = temp;
		enemyCount++;
	}
}

void DeleteEnemy(int _ID)
{
	sfSprite_destroy(enemy[_ID].sprite);
	enemy[_ID].sprite = NULL;
	sfSprite_destroy(enemy[_ID].spriteShadow);
	enemy[_ID].spriteShadow = NULL;
	sfRectangleShape_destroy(enemy[_ID].collider);
	enemy[_ID].collider = NULL;

	for (int i = _ID; i < enemyCount - 1; i++)
	{
		Enemy temp = enemy[i];
		enemy[i] = enemy[i + 1];
		enemy[i + 1] = temp;
	}

	enemyCount--;
}

void ClearEnemy(void)
{
	for (int i = 0; i < enemyCount; i++)
	{
		DeleteEnemy(i);
		enemyCount++;
	}
	enemyCount = 0;
}

void AddMarker(sfVector2f _pos, int _dmg)
{
	if (markerCount < 49)
	{
		Marker temp = { 0 };
		temp.text = sfText_create();
		sfText_setFont(temp.text, font);
		sfText_setOutlineThickness(temp.text, 1);
		sfText_setCharacterSize(temp.text, 50 + (5 * _dmg));
		temp.dmg = _dmg;
		temp.alpha = 255;

		char string[5];
		sprintf_s(string, sizeof(string), "%d", _dmg);
		sfText_setString(temp.text, string);

		sfText_setPosition(temp.text, (sfVector2f) { _pos.x, _pos.y + (10 * markerCount) });

		marker[markerCount] = temp;
		markerCount++;
	}
}

void DeleteMarker(int _ID)
{
	sfText_destroy(marker[_ID].text);
	marker[_ID].text = NULL;

	for (int i = _ID; i < markerCount - 1; i++)
	{
		Marker temp = marker[i];
		marker[i] = marker[i + 1];
		marker[i + 1] = temp;
	}

	markerCount--;
}

void EnemyMove(int _ID, float _dt)
{
	if (!enemy[_ID].isShooting && enemy[_ID].fireRate < 0.7f || enemy[_ID].type == REA_CLOTH)
	{
		sfVector2f pos = sfSprite_getPosition(enemy[_ID].sprite);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(enemy[_ID].sprite);
		float radian = 0;

		//State
		if (enemy[_ID].stateTimer > 0)
		{
			enemy[_ID].stateTimer -= _dt;
		}
		else
		{
			enemy[_ID].state = rand() % 3;
			float ran = (float)(rand() % 11 - 5);
			enemy[_ID].stateTimer = 2 + (ran / 10);
		}

		//States
		switch (enemy[_ID].type)
		{
		case REA_BASE:
		case REA_SHOTGUN:
			enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			switch (enemy[_ID].state)
			{
			case 0:
				radian -= 45;
				break;
			case 1:
				radian -= 45;
				break;
			case 2:
				radian += 45;
				break;
			}
			enemy[_ID].velocity.x = (float)(sin(radian));
			enemy[_ID].velocity.y = (float)(-cos(radian));
			break;
		case REA_CLOTH:
			enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			enemy[_ID].velocity.x = (float)(sin(radian) * enemy[_ID].misc);
			enemy[_ID].velocity.y = (float)(-cos(radian) * enemy[_ID].misc);
			break;
		case SLIME:
			if (enemy[_ID].stateTimer <= 0)
			{
				enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
				enemy[_ID].misc = 1;
			}
			else
			{
				if (enemy[_ID].misc > 0)
				{
					enemy[_ID].misc -= 0.008f;
				}
			}
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			if (radian != 0.000)
			{
				enemy[_ID].velocity.x = (float)(sin(radian) * enemy[_ID].misc);
				enemy[_ID].velocity.y = (float)(-cos(radian) * enemy[_ID].misc);
			}
			break;
		case BAT:
			if (GetDistanceVector2f(pos, GetPlayerPos()) >= 350 || enemy[_ID].velocity.y == 0 || enemy[_ID].velocity.x == 0)
			{
				enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
			}
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			enemy[_ID].velocity.x = (float)(sin(radian));
			enemy[_ID].velocity.y = (float)(-cos(radian));
			break;
		case TORMENTED_SOUL:
			enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			enemy[_ID].velocity.x = (float)(sin(radian) * enemy[_ID].misc);
			enemy[_ID].velocity.y = (float)(-cos(radian) * enemy[_ID].misc);
			break;
		case BIG_CRAB:
			enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			switch (enemy[_ID].state)
			{
			case 0:
				radian = -radian;
				break;
			}
			enemy[_ID].velocity.x = (float)(sin(radian));
			enemy[_ID].velocity.y = (float)(-cos(radian));
			break;
		case TINY_CRAB:
			enemy[_ID].playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
			radian = enemy[_ID].playerRot * (float)(M_PI / 180);
			enemy[_ID].velocity.x = (float)(sin(radian) * 2);
			enemy[_ID].velocity.y = (float)(-cos(radian) * 2);
			break;
		}

		//Collision
		if (enemy[_ID].type != TORMENTED_SOUL && enemy[_ID].type != BAT)
		{
			if (enemy[_ID].velocity.x > 0)
			{
				if (ObjectCollision((sfVector2f) { pos.x + hitbox.width / 2, pos.y }))
				{
					enemy[_ID].velocity.x = 0;
				}
			}
			else
			{
				if (ObjectCollision((sfVector2f) { pos.x - hitbox.width / 2, pos.y }))
				{
					enemy[_ID].velocity.x = 0;
				}
			}
			if (enemy[_ID].velocity.y > 0)
			{
				if (ObjectCollision((sfVector2f) { pos.x, pos.y + hitbox.height / 2 }))
				{
					enemy[_ID].velocity.y = 0;
				}
			}
			else
			{
				if (ObjectCollision((sfVector2f) { pos.x, pos.y - hitbox.height / 2 }))
				{
					enemy[_ID].velocity.y = 0;
				}
			}
		}

		if (GetDistanceVector2f(pos, GetPlayerPos()) > 30 || enemy[_ID].type == BAT || enemy[_ID].type == SLIME)
		{
			if (GetEffect(COWBOY_HAT))
			{
				if (enemyRoped != _ID)
					sfSprite_move(enemy[_ID].sprite, (sfVector2f) { enemy[_ID].velocity.x* enemy[_ID].speed* _dt, enemy[_ID].velocity.y* enemy[_ID].speed* _dt });
				else if (closestDis > 375)
					sfSprite_move(enemy[_ID].sprite, (sfVector2f) { enemy[_ID].velocity.x* enemy[_ID].speed* _dt, enemy[_ID].velocity.y* enemy[_ID].speed* _dt });
			}
			else
				sfSprite_move(enemy[_ID].sprite, (sfVector2f) { enemy[_ID].velocity.x* enemy[_ID].speed* _dt, enemy[_ID].velocity.y* enemy[_ID].speed* _dt });
		}
	}
	else
	{
		enemy[_ID].velocity.x = 0;
		enemy[_ID].velocity.y = 0;
	}
}

void EnemyShoot(int _ID, float _dt)
{
	if (enemy[_ID].fireRate > 0)
	{
		enemy[_ID].fireRate -= _dt;
	}

	//Visual bullshit
	if (abs((int)(LookToDirection(GetPlayerPos(), sfSprite_getPosition(enemy[_ID].sprite)))) < 90)
	{
		sfSprite_setScale(enemy[_ID].sprite, (sfVector2f) { 1, 1 });
	}
	else
	{
		sfSprite_setScale(enemy[_ID].sprite, (sfVector2f) { -1, 1 });
	}

	//Actual Shooting
	sfVector2f pos = { 0 };
	switch (enemy[_ID].type)
	{
	case REA_BASE:
		pos = sfSprite_getPosition(enemy[_ID].sprite);
		if (enemy[_ID].fireRate <= 0)
		{
			AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5), 600, sfTrue, sfFalse);
			if (enemy[_ID].fireThing < 2)
			{
				enemy[_ID].isShooting = sfTrue;
				enemy[_ID].fireRate = 0.2f;
				enemy[_ID].fireThing++;
			}
			else
			{
				enemy[_ID].isShooting = sfFalse;
				float ran = (float)(rand() % 11 - 5);
				enemy[_ID].fireRate = 2.8f + (ran / 10);
				enemy[_ID].fireThing = 0;
			}
		}
		break;
	case REA_SHOTGUN:
		pos = sfSprite_getPosition(enemy[_ID].sprite);
		if (enemy[_ID].fireRate <= 0)
		{
			AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5), 400, sfTrue, sfFalse);
			AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) + 40, 400, sfTrue, sfFalse);
			AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) - 40, 400, sfTrue, sfFalse);
			enemy[_ID].isShooting = sfFalse;
			float ran = (float)(rand() % 11 - 5);
			enemy[_ID].fireRate = 3.2f + (ran / 10);
		}
		break;
	case REA_CLOTH:
		pos = sfSprite_getPosition(enemy[_ID].sprite);
		if (enemy[_ID].fireRate <= 0)
		{
			AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 16 - 10), 600, sfTrue, sfFalse);
			enemy[_ID].isShooting = sfFalse;
			enemy[_ID].fireRate = 0.3f;
		}
		break;
	case BIG_CRAB:
		pos = sfSprite_getPosition(enemy[_ID].sprite);
		pos.y += 45;
		if (enemy[_ID].misc <= 0)
		{
			if (!ObjectCollision(pos))
			{
				AddEnemy(TINY_CRAB, pos, enemy[_ID].id);
				AddEnemyCurrent();
			}
			enemy[_ID].misc = 3.f;
		}
		else
		{
			enemy[_ID].misc -= _dt;
		}
		break;
	default:
		break;
	}

}

void EnemyHurt(int _ID, int _dmg)
{
	if (enemy[_ID].hurtFrame <= 0)
	{
		sfVector2f pos = sfSprite_getPosition(enemy[_ID].sprite);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(enemy[_ID].sprite);
		if (enemy[_ID].life > 0)
		{
			if (_ID == enemyRoped && GetDistanceVector2f(pos, GetPlayerPos()) < 300 && !GetEffect(PASSPORT))
			{
				_dmg *= 2;
			}
			if (_dmg >= 0 && enemy[_ID].type != TORMENTED_SOUL)
			{
				enemy[_ID].life -= _dmg;
			}
			if (GetEffect(PASSPORT))
				enemy[_ID].hurtFrame = 0.10f;
			else
				enemy[_ID].hurtFrame = 0.02f;

			switch (enemy[_ID].type)
			{
			case REA_BASE:
			case REA_SHOTGUN:
			case REA_CLOTH:
				sfSprite_setTextureRect(enemy[_ID].sprite, (sfIntRect) { 0, 75, 75, 75 });
				break;
			case SLIME:
				sfSprite_setTextureRect(enemy[_ID].sprite, (sfIntRect) { 0, 60, 60, 60 });
				break;
			case BAT:
				sfSprite_setTextureRect(enemy[_ID].sprite, (sfIntRect) { 0, 60, 150, 60 });
				break;
			case BIG_CRAB:
				sfSprite_setTextureRect(enemy[_ID].sprite, (sfIntRect) { 0, 95, 220, 95 });
				break;
			case TINY_CRAB:
				sfSprite_setTextureRect(enemy[_ID].sprite, (sfIntRect) { 0, 20, 45, 20 });
				break;
			}
			pos.y -= 50 + hitbox.height / 2;
			AddMarker(pos, _dmg);
		}
		else
		{
			AddMarker(pos, 0);
		}
	}
}

void EnemyDeath(int _ID, float _dt)
{
	if (!enemy[_ID].isDead)
	{
		RemoveEnemyCurrent();
		sfFloatRect hitbox = GetEnemyHitBox(_ID);
		switch (enemy[_ID].type)
		{
		case REA_BASE:
		case REA_SHOTGUN:
		case REA_CLOTH:
		case SLIME:
		case BAT:
			AddNugget(hitbox, 2);
			break;
		case BIG_CRAB:
			AddNugget(hitbox, 8);
			break;
		case TORMENTED_SOUL:
			AddNugget(hitbox, 8);
			break;
		}
		enemy[_ID].isDead = sfTrue;
	}
	if (enemy[_ID].color > 160)
	{
		enemy[_ID].color--;
		sfSprite_setColor(enemy[_ID].sprite, (sfColor) { enemy[_ID].color, enemy[_ID].color, enemy[_ID].color, 255 });

	}
	if (enemy[_ID].type != TINY_CRAB)
	{
		UpdateAnim(_dt, enemy[_ID].anims[DEATH_E]);
	}
	else
	{
		DeleteEnemy(_ID);
	}
}

int GetEnemyCount(void)
{
	return enemyCount;
}

sfFloatRect GetEnemyHitBox(int _ID)
{
	return sfRectangleShape_getGlobalBounds(enemy[_ID].collider);
}

sfBool IsEnemyAlive(int _ID)
{
	if (enemy[_ID].life > 0)
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

int GetEnemyMap(int _ID)
{
	int count = 0;
	for (int i = 0; i < enemyCount; i++)
	{
		if (enemy[i].id == _ID && enemy[i].life > 0)
		{
			count++;
		}
	}
	return count;
}

float GetEnemyHurtFrame(int _ID)
{
	return enemy[_ID].hurtFrame;
}

sfVector2f RopeEnemy(int _ID)
{
	enemyRoped = _ID;
	if (_ID < enemyCount)
	{
		sfVector2f pos = sfRectangleShape_getPosition(enemy[_ID].collider);
		if (enemy[_ID].type != TINY_CRAB)
		{
			SetRope(pos);
		}
		return pos;
	}
	return (sfVector2f) { 0 };
}