#include "Enemy.h"

Enemy enemy[100];
sfTexture* enemyTexture[2];
sfTexture* shadowTexture;
int enemyCount;

void LoadEnemy(void)
{
	enemyCount = 0;
	enemyTexture[REA_BASE] = sfTexture_createFromFile("Assets/Texture/Enemy/rea_base.png", NULL);
	enemyTexture[REA_SHOTGUN] = sfTexture_createFromFile("Assets/Texture/Enemy/rea_shotgun.png", NULL);
	shadowTexture = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);
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
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[REA_BASE], first, 8, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
	}break;
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
		_enemy->anims[DEATH_E] = CreateAnim(enemyTexture[REA_SHOTGUN], first, 8, 1 / 12.0f, _enemy->sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 8.f }, sfFalse);
		_enemy->anims[DEATH_E]->aimOffset = (sfVector2f){ 0 };
		_enemy->anims[DEATH_E]->events = malloc(sizeof(AnimEvent));
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
			sfVector2f pos = sfSprite_getPosition(enemy[i].sprite);
			sfRectangleShape_setPosition(enemy[i].collider, (sfVector2f) { pos.x + 2, pos.y + 2 });
			sfSprite_setPosition(enemy[i].spriteShadow, (sfVector2f) { pos.x + 2, pos.y + 2 });

			if (GetCurrentMap() == enemy[i].id)
			{
				EnemyMove(i, _dt);
				EnemyShoot(i, _dt);
			}
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
		else
		{
			if (!enemy[i].isDead)
			{
				RemoveEnemyCurrent();
				sfFloatRect hitbox = GetEnemyHitBox(i);
				AddNugget(hitbox);
				enemy[i].isDead = sfTrue;
			}
			UpdateAnim(_dt, enemy[i].anims[DEATH_E]);
		}
	}
}

void DrawEnemy(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < enemyCount; i++)
	{
		sfRenderWindow_drawSprite(_window, enemy[i].spriteShadow, NULL);
		sfRenderWindow_drawSprite(_window, enemy[i].sprite, NULL);
		if (_debug)
		{
			sfRenderWindow_drawRectangleShape(_window, enemy[i].collider, NULL);
		}
	}
}

void CleanupEnemy(void)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (enemy[i].anims != NULL)
		{
			for (int y = 0; y < 3; y++)
			{
				if (enemy[i].anims[y] != NULL)
				{
					DestroyAnim(enemy[i].anims[y]);
					enemy[i].anims[y] = NULL;
				}
			}
			free(enemy[i].anims);
			enemy[i].anims = NULL;
		}
	}

	for (int i = 0; i < enemyCount; i++)
	{
		DeleteEnemy(i);
	}
	sfTexture_destroy(enemyTexture[REA_BASE]);
	enemyTexture[REA_BASE] = NULL;
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
		sfRectangleShape_setSize(temp.collider, (sfVector2f) { 40, 60 });
		sfRectangleShape_setOutlineThickness(temp.collider, 3);
		sfRectangleShape_setOutlineColor(temp.collider, sfRed);
		sfRectangleShape_setFillColor(temp.collider, (sfColor) { 0, 0, 0, 0 });
		hitbox = sfRectangleShape_getGlobalBounds(temp.collider);
		sfRectangleShape_setOrigin(temp.collider, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		//Shadow
		temp.spriteShadow = sfSprite_create();
		sfSprite_setTexture(temp.spriteShadow, shadowTexture, sfTrue);
		hitbox = sfSprite_getGlobalBounds(temp.spriteShadow);
		sfSprite_setOrigin(temp.spriteShadow, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		temp.fireRate = 0;
		temp.velocity = (sfVector2f){ 0 };
		temp.id = _idMap;
		temp.type = _type;

		LoadEnemyAnimation(&temp);

		//Type
		switch (_type)
		{
		case REA_BASE:
			temp.life = 5;
			temp.speed = 150;
			break;
		case REA_SHOTGUN:
			temp.life = 5;
			temp.speed = 150;
			break;
		default:
			break;
		}

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
	enemy[_ID].sprite = NULL;

	for (int i = _ID; i < enemyCount - 1; i++)
	{
		Enemy temp = enemy[i];
		enemy[i] = enemy[i + 1];
		enemy[i + 1] = temp;
	}

	enemyCount--;
}

void EnemyMove(int _ID, float _dt)
{
	if (!enemy[_ID].isShooting && enemy[_ID].fireRate < 0.7f)
	{
		sfVector2f pos = sfSprite_getPosition(enemy[_ID].sprite);
		float rotation = LookToDirection(GetPlayerPos(), pos) + 90;
		float radian = rotation * (float)(M_PI / 180);
		enemy[_ID].velocity.x = (float)(sin(radian));
		enemy[_ID].velocity.y = (float)(-cos(radian));
		sfSprite_move(enemy[_ID].sprite, (sfVector2f) { enemy[_ID].velocity.x* enemy[_ID].speed* _dt, enemy[_ID].velocity.y* enemy[_ID].speed* _dt });
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
	switch (enemy[_ID].type)
	{
	case REA_BASE:
		if (GetDistanceVector2f(GetPlayerPos(), sfSprite_getPosition(enemy[_ID].sprite)) < 400 || enemy[_ID].isShooting)
		{
			sfVector2f pos = sfSprite_getPosition(enemy[_ID].sprite);
			if (enemy[_ID].fireRate <= 0)
			{
				AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5), 300, sfTrue);
				if (enemy[_ID].fireThing < 2)
				{
					enemy[_ID].isShooting = sfTrue;
					enemy[_ID].fireRate = 0.2f;
					enemy[_ID].fireThing++;
				}
				else
				{
					enemy[_ID].isShooting = sfFalse;
					enemy[_ID].fireRate = 1.2f;
					enemy[_ID].fireThing = 0;
				}
			}
		}
		break;
	case REA_SHOTGUN:
		if (GetDistanceVector2f(GetPlayerPos(), sfSprite_getPosition(enemy[_ID].sprite)) < 400 || enemy[_ID].isShooting)
		{
			sfVector2f pos = sfSprite_getPosition(enemy[_ID].sprite);
			if (enemy[_ID].fireRate <= 0)
			{
				AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5), 300, sfTrue);
				AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) + 20, 300, sfTrue);
				AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) - 20, 300, sfTrue);
				enemy[_ID].isShooting = sfFalse;
				enemy[_ID].fireRate = 2.f;
			}
		}
		break;
	default:
		break;
	}

}

void EnemyHandThing(int _ID, sfVector2f _pos)
{
}

void EnemyHurt(int _ID)
{
	if (enemy[_ID].life > 0)
	{
		enemy[_ID].life--;
		enemy[_ID].hurtFrame = 0.02f;
		sfSprite_setTextureRect(enemy[_ID].sprite, (sfIntRect) { 0, 75, 75, 75 });
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
