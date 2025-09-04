#include "Boss.h"

Boss activeBoss;
Nakrom nakrom;
OldGuard oldGuard;
sfTexture* bossTexture[3];
sfTexture* shadowBoss;
int id;

void LoadBoss(void)
{
	activeBoss = NONE;
	bossTexture[0] = sfTexture_createFromFile("Assets/Texture/Enemy/nakrom_the_ancient.png", NULL);
	bossTexture[1] = sfTexture_createFromFile("Assets/Texture/Enemy/old_guard.png", NULL);
	shadowBoss = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);
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
			sfSprite_setPosition(nakrom.shadow, (sfVector2f) { pos.x, pos.y + hitbox.height / 2 - 40 });
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

				if (nakrom.hp < 65 && nakrom.spawnMax < 1)
				{
					nakrom.spawnMax = 1;
					nakrom.spawn = sfTrue;
				}
				else if (nakrom.hp < 35 && nakrom.spawnMax < 2)
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
					PlaySound(nakrom.summonSound);
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

		case OLD_GUARD:
			pos = sfSprite_getPosition(oldGuard.sprite);
			hitbox = sfSprite_getGlobalBounds(oldGuard.sprite);
			sfRectangleShape_setPosition(oldGuard.collider, (sfVector2f) { pos.x, pos.y });
			sfSprite_setPosition(oldGuard.shadow, (sfVector2f) { pos.x, pos.y + hitbox.height / 2 - 70 });
			UpdateAnim(_dt, oldGuard.anims[oldGuard.state]);

			if (oldGuard.hp > 0)
			{
				if (abs((int)(LookToDirection(GetPlayerPos(), sfSprite_getPosition(oldGuard.sprite)))) < 90)
				{
					sfSprite_setScale(oldGuard.sprite, (sfVector2f) { 1, 1 });
				}
				else
				{
					sfSprite_setScale(oldGuard.sprite, (sfVector2f) { -1, 1 });
				}
				if (oldGuard.hurtFrame > 0)
				{
					oldGuard.hurtFrame -= _dt;
				}
			}
			else
			{
				//Death
				oldGuard.state = DEATH_BOSS;
				if (oldGuard.color > 160)
				{
					oldGuard.color--;
					sfSprite_setColor(oldGuard.sprite, (sfColor) { oldGuard.color, oldGuard.color, oldGuard.color, 255 });
				}
			}

			switch (oldGuard.state)
			{
			case IDLE_BOSS:
				if (oldGuard.stateTimer <= 0)
				{
					if (!oldGuard.dash)
					{
						if (oldGuard.attackType < 2)
							oldGuard.attackType += 1;
						else
							oldGuard.attackType = 0;
						oldGuard.state = ATTACK2_BOSS;
						oldGuard.rotShoot = (int)LookToDirection(GetPlayerPos(), pos);
						oldGuard.stateTimer = 5;
						if (oldGuard.hp < 50 )
							oldGuard.anims[ATTACK_STARTUP]->rate = 1 / 12.0f;
						if (oldGuard.hp < 150 && oldGuard.hp > 50)
							oldGuard.stateTimer /= 2;
					}
					else
					{
						ResetAnim(oldGuard.anims[ATTACK_STARTUP]);
						oldGuard.state = ATTACK_STARTUP;
					}
				}
				else
					oldGuard.stateTimer -= _dt;
				break;
			case ATTACK_BOSS:
				radian = oldGuard.rotShoot * (float)(M_PI / 180);
				oldGuard.velocity.x = (float)(sin(radian));
				oldGuard.velocity.y = (float)(-cos(radian));

				if (oldGuard.dash)
					sfSprite_move(oldGuard.sprite, (sfVector2f) { oldGuard.velocity.x * 2000 * _dt, oldGuard.velocity.y * 2000 * _dt });
				else
					sfSprite_move(oldGuard.sprite, (sfVector2f) { -(oldGuard.velocity.x * 2000 * _dt), -(oldGuard.velocity.y * 2000 * _dt) });

				if (GetDistanceVector2f(pos, oldGuard.originalPoint) < 50 && !oldGuard.dash)
				{
					oldGuard.state = IDLE;
					if (oldGuard.hp > 50)
						oldGuard.stateTimer = 3;
					if (oldGuard.hp < 150 && oldGuard.hp > 50)
						oldGuard.stateTimer /= 2;
				}
				if (MoveCollision(hitbox))
					oldGuard.dash = sfFalse;

				break;
			case ATTACK2_BOSS:
				switch (oldGuard.attackType)
				{
				case 0:
					if (oldGuard.fireRate <= 0)
					{
						oldGuard.fireRate = 0.005f;
						AddBullet(pos, (float)oldGuard.rotShoot + rand() % 10 - 5, 200, sfTrue, sfFalse);
						AddBullet(pos, (float)oldGuard.rotShoot + rand() % 10 - 5, 200, sfTrue, sfFalse);
						AddBullet(pos, (float)oldGuard.rotShoot + rand() % 10 - 5, 200, sfTrue, sfFalse);
						oldGuard.rotShoot += 5;
					}
					else
						oldGuard.fireRate -= _dt;
					break;
				case 1:
					if (oldGuard.fireRate <= 0)
					{
						oldGuard.fireRate = 0.1f;
						for (int i = 0; i < 9; i++)
						{
							AddBullet(pos, (float)LookToDirection(GetPlayerPos(), pos) + 90 + (40 * i), 300, sfTrue, sfFalse);
						}
						oldGuard.rotShoot += 5;
					}
					else
						oldGuard.fireRate -= _dt;
					break;
				case 2:
					if (oldGuard.fireRate <= 0)
					{
						oldGuard.fireRate = 1.5f;
						for (int i = 0; i < 360; i += 2)
						{
							AddBullet(pos, (float)i, 300, sfTrue, sfFalse);
						}
						oldGuard.rotShoot += 5;
					}
					else
						oldGuard.fireRate -= _dt;
					break;
				default:
					break;
				}
				if (oldGuard.stateTimer <= 0)
				{
					oldGuard.state = IDLE_BOSS;
					oldGuard.dash = sfTrue;
					if (oldGuard.hp > 50)
						oldGuard.stateTimer = 3;
					if (oldGuard.hp < 150 && oldGuard.hp > 50)
						oldGuard.stateTimer /= 2;
				}
				else
					oldGuard.stateTimer -= _dt;
				break;
			case DEATH_BOSS:
				if (oldGuard.anims[3]->frameNum < 3)
				{
					AddNugget(hitbox, 1);
				}
				break;
			case ATTACK_STARTUP:
				if (oldGuard.anims[ATTACK_STARTUP]->frameNum < 5)
				{
					oldGuard.rotShoot = (int)LookToDirection(GetPlayerPos(), pos) + 90;
				}
				if (IsFinishedAnim(oldGuard.anims[ATTACK_STARTUP]))
				{
					oldGuard.state = ATTACK_BOSS;
				}
				break;
			}

			break;
		}
	}
}

void DrawBoss(sfRenderWindow* _window, sfBool _debug)
{
	switch (activeBoss)
	{
	case NAKROM:
		sfRenderWindow_drawSprite(_window, nakrom.sprite, NULL);
		if (_debug)
			sfRenderWindow_drawRectangleShape(_window, nakrom.collider, NULL);
		sfRenderWindow_drawSprite(_window, nakrom.shadow, NULL);
		break;
	case OLD_GUARD:
		sfRenderWindow_drawSprite(_window, oldGuard.sprite, NULL);
		if (_debug)
			sfRenderWindow_drawRectangleShape(_window, oldGuard.collider, NULL);
		sfRenderWindow_drawSprite(_window, oldGuard.shadow, NULL);
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
		nakrom.hp = 80;
		nakrom.color = 255;
		nakrom.stateTimer = 3;

		nakrom.summonSound = GetSoundCount();
		AddSound(S_NAKROMSUMMON);

		nakrom.sprite = sfSprite_create();
		sfSprite_setTexture(nakrom.sprite, bossTexture[0], sfTrue);
		sfFloatRect hitbox = sfSprite_getGlobalBounds(nakrom.sprite);
		sfSprite_setOrigin(nakrom.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		sfSprite_setPosition(nakrom.sprite, _position);

		nakrom.shadow = sfSprite_create();
		sfSprite_setTexture(nakrom.shadow, shadowBoss, sfTrue);
		hitbox = sfSprite_getGlobalBounds(nakrom.shadow);
		sfSprite_setOrigin(nakrom.shadow, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		nakrom.collider = sfRectangleShape_create();
		sfRectangleShape_setOutlineThickness(nakrom.collider, 3);
		sfRectangleShape_setOutlineColor(nakrom.collider, sfRed);
		sfRectangleShape_setFillColor(nakrom.collider, (sfColor) { 0, 0, 0, 0 });
		sfRectangleShape_setSize(nakrom.collider, (sfVector2f) { 100, 100 });
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
	case OLD_GUARD:
		oldGuard.hp = 200;
		oldGuard.color = 255;
		oldGuard.stateTimer = 5;

		oldGuard.sprite = sfSprite_create();
		sfSprite_setTexture(oldGuard.sprite, bossTexture[1], sfTrue);
		hitbox = sfSprite_getGlobalBounds(oldGuard.sprite);
		sfSprite_setOrigin(oldGuard.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		sfSprite_setPosition(oldGuard.sprite, _position);

		oldGuard.shadow = sfSprite_create();
		sfSprite_setTexture(oldGuard.shadow, shadowBoss, sfTrue);
		hitbox = sfSprite_getGlobalBounds(oldGuard.shadow);
		sfSprite_setOrigin(oldGuard.shadow, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		oldGuard.originalPoint = _position;

		oldGuard.collider = sfRectangleShape_create();
		sfRectangleShape_setOutlineThickness(oldGuard.collider, 3);
		sfRectangleShape_setOutlineColor(oldGuard.collider, sfRed);
		sfRectangleShape_setFillColor(oldGuard.collider, (sfColor) { 0, 0, 0, 0 });
		sfRectangleShape_setSize(oldGuard.collider, (sfVector2f) { 100, 100 });
		hitbox = sfRectangleShape_getGlobalBounds(oldGuard.collider);
		sfRectangleShape_setOrigin(oldGuard.collider, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
		oldGuard.anims = calloc(5, sizeof(Anim*));
		if (oldGuard.anims != NULL)
		{
			sfVector2u size = sfTexture_getSize(bossTexture[1]);
			sfIntRect first = { 0, 0, size.x / 6, size.y / 6 };
			oldGuard.anims[IDLE_BOSS] = CreateAnim(bossTexture[1], first, 5, 1 / 6.0f, oldGuard.sprite, (sfVector2f) { (float)size.x / 12.f, (float)size.y / 10.f }, sfTrue);
			oldGuard.anims[IDLE_BOSS]->aimOffset = (sfVector2f){ 0 };
			oldGuard.anims[IDLE_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 210 * 3, size.x / 6, size.y / 6 };
			oldGuard.anims[ATTACK_BOSS] = CreateAnim(bossTexture[1], first, 4, 1 / 6.0f, oldGuard.sprite, (sfVector2f) { (float)size.x / 12.f, (float)size.y / 10.f }, sfTrue);
			oldGuard.anims[ATTACK_BOSS]->aimOffset = (sfVector2f){ 0 };
			oldGuard.anims[ATTACK_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 210 * 4, size.x / 6, size.y / 6 };
			oldGuard.anims[ATTACK2_BOSS] = CreateAnim(bossTexture[1], first, 5, 1 / 6.0f, oldGuard.sprite, (sfVector2f) { (float)size.x / 12.f, (float)size.y / 10.f }, sfTrue);
			oldGuard.anims[ATTACK2_BOSS]->aimOffset = (sfVector2f){ 0 };
			oldGuard.anims[ATTACK2_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 210 * 5, size.x / 6, size.y / 6 };
			oldGuard.anims[DEATH_BOSS] = CreateAnim(bossTexture[1], first, 6, 1 / 6.0f, oldGuard.sprite, (sfVector2f) { (float)size.x / 12.f, (float)size.y / 10.f }, sfFalse);
			oldGuard.anims[DEATH_BOSS]->aimOffset = (sfVector2f){ 0 };
			oldGuard.anims[DEATH_BOSS]->events = malloc(sizeof(AnimEvent));

			first = (sfIntRect){ 0, 210 * 2, size.x / 6, size.y / 6 };
			oldGuard.anims[ATTACK_STARTUP] = CreateAnim(bossTexture[1], first, 6, 1 / 6.0f, oldGuard.sprite, (sfVector2f) { (float)size.x / 12.f, (float)size.y / 10.f }, sfFalse);
			oldGuard.anims[ATTACK_STARTUP]->aimOffset = (sfVector2f){ 0 };
			oldGuard.anims[ATTACK_STARTUP]->events = malloc(sizeof(AnimEvent));
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
		sfSprite_destroy(nakrom.shadow);
		nakrom.shadow = NULL;
		sfRectangleShape_destroy(nakrom.collider);
		nakrom.collider = NULL;
		DeleteSound(nakrom.summonSound);
		break;
	case OLD_GUARD:
		sfSprite_destroy(oldGuard.sprite);
		oldGuard.sprite = NULL;
		sfSprite_destroy(oldGuard.shadow);
		oldGuard.shadow = NULL;
		sfRectangleShape_destroy(oldGuard.collider);
		oldGuard.collider = NULL;
		break;
	}
	activeBoss = NONE;
}

void HurtBoss(int _dmg)
{

	sfVector2f pos = { 0 };
	sfFloatRect hitbox = { 0 };
	switch (activeBoss)
	{
	case NAKROM:
		if (nakrom.hurtFrame <= 0)
		{
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
		}
		break;
	case OLD_GUARD:
		if (oldGuard.hurtFrame <= 0)
		{
			pos = sfSprite_getPosition(oldGuard.sprite);
			hitbox = sfSprite_getGlobalBounds(oldGuard.sprite);
			if (GetEffect(COWBOY_HAT) && GetDistanceVector2f(pos, GetPlayerPos()) < 300 && !GetEffect(PASSPORT))
			{
				_dmg *= 2;
			}
			if (_dmg > 0)
			{
				oldGuard.hp -= _dmg;
				oldGuard.hurtFrame = 0.10f;
			}
		}
		break;
	}
	pos.y -= 50 + hitbox.height / 2;
	AddMarker(pos, _dmg);
}

void RopeBoss(void)
{
	sfVector2f pos = { 0 };
	switch (activeBoss)
	{
	case NAKROM:
		pos = sfRectangleShape_getPosition(nakrom.collider);
	case OLD_GUARD:
		pos = sfRectangleShape_getPosition(oldGuard.collider);
	}

	SetRope(pos);
	RopeEnemy(9999);
}

sfFloatRect GetBossHitbox(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return sfSprite_getGlobalBounds(nakrom.sprite);
		break;
	case OLD_GUARD:
		return sfSprite_getGlobalBounds(oldGuard.sprite);
		break;
	}
	return (sfFloatRect) { 0 };
}

sfBool GetBossDead(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return nakrom.hp <= 0;
		break;
	case OLD_GUARD:
		return oldGuard.hp <= 0;
		break;
	}
	return sfFalse;
}

int GetBossHP(void)
{
	switch (activeBoss)
	{
	case NAKROM:
		return nakrom.hp;
		break;
	case OLD_GUARD:
		return oldGuard.hp;
		break;
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
		break;
	case OLD_GUARD:
		return oldGuard.hurtFrame;
		break;
	}
	return 0;
}

int GetBossID(void)
{
	return id;
}
