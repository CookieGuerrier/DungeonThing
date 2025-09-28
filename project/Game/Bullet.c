#include "Bullet.h"

Bullet bullet[600];
int bulletCount;
sfTexture* textureBullet;
int bulletSound;

void LoadBullet(void)
{
	bulletCount = 0;
	textureBullet = sfTexture_createFromFile("Assets/Texture/Player/bullet.png", NULL);
	bulletSound = GetSoundCount();
	AddSound(S_BULLET);
}

void UpdateBullet(float _dt, sfRenderWindow* _window)
{
	for (int i = 0; i < bulletCount; i++)
	{
		if (!bullet[i].death)
		{
			sfSprite_move(bullet[i].sprite, (sfVector2f) { bullet[i].velocity.x* (_dt * bullet[i].speed), bullet[i].velocity.y* (_dt * bullet[i].speed) });
			//Wall collision
			sfFloatRect hitbox = sfSprite_getGlobalBounds(bullet[i].sprite);

			//Enemy Collision 
			if (!bullet[i].friendlyFire)
			{
				//Effects
				if (GetEffect(BOOMERANG) && bullet[i].speed > -2000)
				{
					bullet[i].speed -= 50;
				}

				for (int y = 0; y < GetEnemyCount(); y++)
				{
					if (IsEnemyAlive(y))
					{
						sfFloatRect enemy = GetEnemyHitBox(y);
						if (sfFloatRect_intersects(&enemy, &hitbox, NULL))
						{
							//Effects
							if (bullet[i].speed < 0)
							{
								bullet[i].dmg++;
							}
							if (GetEffect(CHRONO))
							{
								bullet[i].dmg += abs(bullet[i].speed / 1000) - 1;
							}
							if (GetEffect(COWBOY_HAT))
							{
								RopeEnemy(y);
							}
							if (GetEffect(PASSPORT))
							{
								bullet[i].dmg = 1;
							}

							//Collis
							EnemyHurt(y, bullet[i].dmg);
							if (!GetEffect(PASSPORT))
							{
								bullet[i].death = sfTrue;
							}
						}
					}
				}

				//Boss
				sfFloatRect boss = GetBossHitbox();
				if (sfFloatRect_intersects(&boss, &hitbox, NULL) && !GetBossDead())
				{
					//Effects
					if (bullet[i].speed < 0)
					{
						bullet[i].dmg++;
					}
					if (GetEffect(CHRONO))
					{
						bullet[i].dmg += abs(bullet[i].speed / 1000) - 1;
					}
					if (GetEffect(COWBOY_HAT))
					{
						RopeBoss();
					}
					if (GetEffect(PASSPORT))
					{
						bullet[i].dmg = 1;
					}

					//Collis
					HurtBoss(bullet[i].dmg);
					if (!GetEffect(PASSPORT))
					{
						bullet[i].death = sfTrue;
					}
				}
			}
			else
			{
				sfFloatRect player = GetPlayerHitbox();
				if (sfFloatRect_intersects(&player, &hitbox, NULL) && GetRoll() <= 0.3f)
				{
					LoseLife(1);
					bullet[i].death = sfTrue;
				}
			}

			if (BulletCollision(hitbox, &bullet[i].velocity) || GetBulletMap(hitbox) != GetCurrentMap())
			{
				if (!bullet[i].friendlyFire)
					PlaySound(bulletSound);
				if (bullet[i].bounce > 0)
				{
					bullet[i].bounce--;
					if (bullet[i].speed < 0)
					{
						bullet[i].speed -= 1000;
					}
					else
					{
						bullet[i].speed += 1000;
					}
				}
				else
				{
					bullet[i].death = sfTrue;
				}
			}
		}
		else if (!IsFinishedAnim(bullet[i].anims[0]))
		{
			UpdateAnim(_dt, bullet[i].anims[0]);
		}
		else
		{
			DeleteBullet(i);
		}
	}

}

void DrawBullet(sfRenderWindow* _window, sfBool _debug)
{
	for (int i = 0; i < bulletCount; i++)
	{
		sfRenderWindow_drawSprite(_window, bullet[i].sprite, NULL);
	}
}

void CleanupBullet(void)
{
	for (int i = 0; i < bulletCount; i++)
	{
		DeleteBullet(i);
		bulletCount++;
	}
	sfTexture_destroy(textureBullet);
	textureBullet = NULL;
	DeleteSound(bulletSound);
}

void AddBullet(sfVector2f _pos, float _rot, int _speed, sfBool _friendlyFire, sfBool _bigBullet)
{
	if (bulletCount < 600)
	{
		Bullet temp = { 0 };

		temp.sprite = sfSprite_create();
		sfSprite_setTexture(temp.sprite, textureBullet, sfTrue);
		if (_bigBullet)
		{
			sfSprite_setScale(temp.sprite, (sfVector2f) { 2, 2 });
		}
		sfSprite_setPosition(temp.sprite, _pos);
		sfSprite_setRotation(temp.sprite, _rot + 91);
		temp.dmg = 1;

		sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
		sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		temp.rotation = _rot;
		float playerRadian = _rot * (float)(M_PI / 180);
		temp.velocity.x = (float)(sin(playerRadian));
		temp.velocity.y = (float)(-cos(playerRadian));

		temp.friendlyFire = _friendlyFire;
		if (_friendlyFire)
		{
			sfSprite_setColor(temp.sprite, (sfColor) { 255, 0, 255, 255 });
		}
		else
		{
			//Effects
			if (GetEffect(BLUE_GEL))
			{
				temp.bounce += 2;
			}
			if (GetEffect(CONTRACT))
			{
				if (GetHP() >= 3)
				{
					temp.dmg -= GetHP() - 3;
				}
				else if (GetHP() < 3)
				{
					temp.dmg += (3 - GetHP());
				}
			}
			if (GetEffect(BATTERY))
			{
				sfSprite_setScale(temp.sprite, (sfVector2f) { 2, 2 });
				temp.dmg += 2;
				temp.speed += 1000;
			}
		}

		temp.anims = calloc(1, sizeof(Anim*));
		if (temp.anims != NULL)
		{
			sfVector2u size = sfTexture_getSize(textureBullet);
			sfIntRect first = { 0, 0, size.x / 5, size.y };
			temp.anims[0] = CreateAnim(textureBullet, first, 5, 1 / 32.0f, temp.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 8.f }, sfFalse);
			temp.anims[0]->aimOffset = (sfVector2f){ 0 };
			temp.anims[0]->events = malloc(sizeof(AnimEvent));
		}
		temp.speed += _speed;

		bullet[bulletCount] = temp;
		bulletCount++;
	}
}

void DeleteBullet(int _ID)
{
	if (bulletCount > 0)
	{
		sfSprite_destroy(bullet[_ID].sprite);
		bullet[_ID].sprite = NULL;

		for (int i = _ID; i < bulletCount - 1; i++)
		{
			Bullet temp = bullet[i];
			bullet[i] = bullet[i + 1];
			bullet[i + 1] = temp;
		}

		bulletCount--;
	}
}

void ClearBullet(void)
{
	for (int i = 0; i < bulletCount; i++)
	{
		DeleteBullet(i);
		bulletCount++;
	}
	bulletCount = 0;
}
