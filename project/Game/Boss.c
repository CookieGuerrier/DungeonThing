#include "Boss.h"

Boss activeBoss;
Nakrom nakrom;
sfTexture* bossTexture[3];
int id;

void LoadBoss(void)
{
	activeBoss = NONE;
	bossTexture[0] = sfTexture_createFromFile("Assets/Texture/Enemy/nakrom_the_ancient.png", NULL);
}

void UpdateBoss(float _dt, sfRenderWindow* _window)
{
	sfVector2f pos;
	sfFloatRect hitbox;
	float radian = 0;
	float playerRot = 0;
	if (GetCurrentMap() == id)
	{
		switch (activeBoss)
		{
		case NAKROM:
			//Nakrom
			pos = sfSprite_getPosition(nakrom.sprite);
			hitbox = sfSprite_getGlobalBounds(nakrom.sprite);
			sfRectangleShape_setPosition(nakrom.collider, (sfVector2f) { pos.x, pos.y });
			UpdateAnim(_dt, nakrom.anims[nakrom.state]);
			if (nakrom.hp > 0)
			{
				if (abs((int)(LookToDirection(GetPlayerPos(), sfSprite_getPosition(nakrom.sprite)))) < 90)
				{
					sfSprite_setScale(nakrom.sprite, (sfVector2f) { 1, 1 });
				}
				else
				{
					sfSprite_setScale(nakrom.sprite, (sfVector2f) { -1, 1 });
				}
				if (nakrom.hurtFrame > 0)
				{
					nakrom.hurtFrame -= _dt;
				}

				if (nakrom.hp < 70 && nakrom.spawnMax < 1)
				{
					nakrom.spawnMax = 1;
					nakrom.spawn = sfTrue;
				}
				else if (nakrom.hp < 40 && nakrom.spawnMax < 2)
				{
					nakrom.spawnMax = 2;
					nakrom.spawn = sfTrue;
				}
			}
			else
			{
				//Death
				nakrom.state = DEATH_BOSS;
				if (nakrom.color > 160)
				{
					nakrom.color--;
					sfSprite_setColor(nakrom.sprite, (sfColor) { nakrom.color, nakrom.color, nakrom.color, 255 });
				}
			}

			switch (nakrom.state)
			{
			case IDLE_BOSS:
				//Movement
				if (GetDistanceVector2f(pos, GetPlayerPos()) > 50)
				{
					playerRot = LookToDirection(GetPlayerPos(), pos) + 90;
					radian = playerRot * (float)(M_PI / 180);
					nakrom.velocity.x = (float)(sin(radian));
					nakrom.velocity.y = (float)(-cos(radian));
					sfSprite_move(nakrom.sprite, (sfVector2f) { nakrom.velocity.x * 50 * _dt, nakrom.velocity.y * 50 * _dt });
				}
				//Attack 1
				if (nakrom.stateTimer < 0)
				{
					nakrom.anims[1]->frameNum = 0;
					nakrom.anims[2]->frameNum = 0;
					nakrom.stateTimer = 5;
					if (nakrom.spawn)
						nakrom.state = ATTACK2_BOSS;
					else
						nakrom.state = ATTACK_BOSS;
				}
				else
				{
					nakrom.stateTimer -= _dt;
				}
				//Classic shoot
				switch (nakrom.fireState)
				{
				case 0:
					if (nakrom.fireRate <= 0)
					{
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5), 600, sfTrue, sfFalse);
						if (nakrom.fireThing < 2)
						{
							nakrom.fireRate = 0.2f;
							nakrom.fireThing++;
						}
						else
						{
							nakrom.fireRate = 2;
							nakrom.fireThing = 0;
						}
					}
					break;
				case 1:
					if (nakrom.fireRate <= 0)
					{
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5), 400, sfTrue, sfFalse);
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) + 20, 400, sfTrue, sfFalse);
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) - 20, 400, sfTrue, sfFalse);
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) + 60, 400, sfTrue, sfFalse);
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 11 - 5) - 60, 400, sfTrue, sfFalse);
						nakrom.fireRate = 2;
					}
					break;
				case 2:
					if (nakrom.fireRate <= 0)
					{
						AddBullet(pos, LookToDirection(GetPlayerPos(), pos) + 90 + (rand() % 30 - 20), 800, sfTrue, sfFalse);
						nakrom.fireRate = 0.1f;
					}
					break;
				}
				if (nakrom.fireRate > 0)
					nakrom.fireRate -= _dt;
				break;
			case ATTACK_BOSS:
				//Circle attack
				if (IsFinishedAnim(nakrom.anims[1]))
				{
					for (int i = 0; i < 360; i += 6)
					{
						AddBullet(pos, (float)i, 400, sfTrue, sfFalse);
					}
					nakrom.state = IDLE_BOSS;
					nakrom.fireState = rand() % 3;
					nakrom.fireRate = 2;
				}
				break;
			case ATTACK2_BOSS:
				if (IsFinishedAnim(nakrom.anims[2]))
				{
					pos.y += 10;
					pos.x -= 20;
					AddEnemy(rand() % 3, pos, id);
					AddEnemyCurrent();
					pos.x += 40;
					AddEnemy(rand() % 3, pos, id);
					AddEnemyCurrent();
					nakrom.spawn = sfFalse;
					nakrom.state = IDLE_BOSS;
				}
				break;
			case DEATH_BOSS:
				if (nakrom.anims[3]->frameNum < 3)
				{
					AddNugget(hitbox, 1);
				}
				break;
			}
			break;
		}
	}
}

void DrawBoss(sfRenderWindow* _window, sfBool _debug)
{
	switch(activeBoss)
	{
	case NAKROM:
		sfRenderWindow_drawSprite(_window, nakrom.sprite, NULL);
		if (_debug)
			sfRenderWindow_drawRectangleShape(_window, nakrom.collider, NULL);
		break;
	}
}

void CleanupBoss(void)
{
	for (int i = 0; i < 3; i++)
	{
		sfTexture_destroy(bossTexture[i]);
		bossTexture[i] = NULL;
	}
	DeleteBoss();
}

void AddBoss(Boss _boss, sfVector2f _position, int _id)
{
	id = _id;
	activeBoss = _boss;
	switch (_boss)
	{
	case NAKROM:
		nakrom.hp = 100;
		nakrom.color = 255;
		nakrom.stateTimer = 5;

		nakrom.sprite = sfSprite_create();
		sfSprite_setTexture(nakrom.sprite, bossTexture[0], sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(nakrom.sprite);
		sfSprite_setOrigin(nakrom.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		sfSprite_setPosition(nakrom.sprite, _position);

		nakrom.collider = sfRectangleShape_create();
		sfRectangleShape_setOutlineThickness(nakrom.collider, 3);
		sfRectangleShape_setOutlineColor(nakrom.collider, sfRed);
		sfRectangleShape_setFillColor(nakrom.collider, (sfColor) { 0, 0, 0, 0 });
		sfRectangleShape_setSize(nakrom.collider, (sfVector2f) { 100, 130 });
		hitbox = sfRectangleShape_getGlobalBounds(nakrom.collider);
		sfRectangleShape_setOrigin(nakrom.collider, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		nakrom.anims = calloc(4, sizeof(Anim*));
		if (nakrom.anims != NULL)
		{
			sfVector2u size = sfTexture_getSize(bossTexture[0]);
			sfIntRect first = { 0, 0, size.x / 8, size.y / 5 };
			nakrom.anims[IDLE_BOSS] = CreateAnim(bossTexture[0], first, 5, 1 / 6.0f, nakrom.sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 10.f }, sfTrue);
			nakrom.anims[IDLE_BOSS]->aimOffset = (sfVector2f){ 0 };
			nakrom.anims[IDLE_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 150 * 2, size.x / 8, size.y / 5 };
			nakrom.anims[ATTACK_BOSS] = CreateAnim(bossTexture[0], first, 5, 1 / 6.0f, nakrom.sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 10.f }, sfFalse);
			nakrom.anims[ATTACK_BOSS]->aimOffset = (sfVector2f){ 0 };
			nakrom.anims[ATTACK_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 150 * 3, size.x / 8, size.y / 5 };
			nakrom.anims[ATTACK2_BOSS] = CreateAnim(bossTexture[0], first, 5, 1 / 6.0f, nakrom.sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 10.f }, sfFalse);
			nakrom.anims[ATTACK2_BOSS]->aimOffset = (sfVector2f){ 0 };
			nakrom.anims[ATTACK2_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 150 * 4, size.x / 8, size.y / 5 };
			nakrom.anims[DEATH_BOSS] = CreateAnim(bossTexture[0], first, 8, 1 / 6.0f, nakrom.sprite, (sfVector2f) { (float)size.x / 16.f, (float)size.y / 10.f }, sfFalse);
			nakrom.anims[DEATH_BOSS]->aimOffset = (sfVector2f){ 0 };
			nakrom.anims[DEATH_BOSS]->events = malloc(sizeof(AnimEvent));
		}
		break;
	}
}

void DeleteBoss(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		sfSprite_destroy(nakrom.sprite);
		nakrom.sprite = NULL;
		sfRectangleShape_destroy(nakrom.collider);
		nakrom.collider = NULL;
		break;
	}
	activeBoss = NONE;
}

void HurtBoss(int _dmg)
{
	if (nakrom.hurtFrame <= 0)
	{

		sfVector2f pos = { 0 };
		sfFloatRect hitbox = { 0 };
		switch (activeBoss)
		{
		case NAKROM:
			pos = sfSprite_getPosition(nakrom.sprite);
			hitbox = sfSprite_getGlobalBounds(nakrom.sprite);
			if (GetEffect(COWBOY_HAT) && GetDistanceVector2f(pos, GetPlayerPos()) < 300 && !GetEffect(PASSPORT))
			{
				_dmg *= 2;
			}
			if (_dmg > 0)
			{
				nakrom.hp -= _dmg;
				nakrom.hurtFrame = 0.10f;
			}
			break;
		}
		pos.y -= 50 + hitbox.height / 2;
		AddMarker(pos, _dmg);
	}
}

void RopeBoss(void)
{
	sfVector2f pos = sfRectangleShape_getPosition(nakrom.collider);
	SetRope(pos);
	RopeEnemy(9999);
}

sfFloatRect GetBossHitbox(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return sfSprite_getGlobalBounds(nakrom.sprite);
	}
	return (sfFloatRect) { 0 };
}

sfBool GetBossDead(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return nakrom.hp <= 0;
	}
	return sfFalse;
}

int GetBossHP(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return nakrom.hp;
	}
	return 0;
}

int GetBossActive(void)
{
	return activeBoss;
}

float GetBossHurtFrame(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return nakrom.hurtFrame;
	}
	return 0;
}

int GetBossID(void)
{
	return id;
}
