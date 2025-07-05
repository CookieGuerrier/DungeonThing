#include "Bullet.h"

Bullet bullet[300];
int bulletCount;
sfTexture* textureBullet;

void LoadBullet(void)
{
	bulletCount = 0;
	textureBullet = sfTexture_createFromFile("Assets/Texture/Player/bullet.png", NULL);
}

void UpdateBullet(float _dt, sfRenderWindow* _window)
{
	for (int i = 0; i < bulletCount; i++)
	{
		sfSprite_move(bullet[i].sprite, (sfVector2f) { bullet[i].velocity.x* (_dt * bullet[i].speed), bullet[i].velocity.y* (_dt * bullet[i].speed) });

		//Wall collision
		sfFloatRect hitbox = sfSprite_getGlobalBounds(bullet[i].sprite);

		//Enemy Collision
		if (!bullet[i].friendlyFire)
		{
			for (int y = 0; y < GetEnemyCount(); y++)
			{
				if (IsEnemyAlive(y))
				{
					sfFloatRect enemy = GetEnemyHitBox(y);
					if (sfFloatRect_intersects(&enemy, &hitbox, NULL))
					{
						EnemyHurt(y);
						DeleteBullet(i);
					}
				}
			}
		}
		else
		{
			sfFloatRect player = GetPlayerHitbox();
			if (sfFloatRect_intersects(&player, &hitbox, NULL))
			{
				LoseLife(1);
				DeleteBullet(i);
			}
		}

		if (BulletCollision(hitbox) || GetBulletMap(hitbox) != GetCurrentMap())
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
}

void AddBullet(sfVector2f _pos, float _rot, int _speed, sfBool _friendlyFire)
{
	if (bulletCount < 300)
	{
		Bullet temp = { 0 };

		temp.sprite = sfSprite_create();
		sfSprite_setTexture(temp.sprite, textureBullet, sfTrue);
		sfSprite_setPosition(temp.sprite, _pos);
		sfSprite_setRotation(temp.sprite, _rot + 91);

		sfFloatRect hitbox = sfSprite_getGlobalBounds(temp.sprite);
		sfSprite_setOrigin(temp.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

		float playerRadian = _rot * (float)(M_PI / 180);
		temp.velocity.x = (float)(sin(playerRadian));
		temp.velocity.y = (float)(-cos(playerRadian));

		temp.friendlyFire = _friendlyFire;
		temp.speed = _speed;

		bullet[bulletCount] = temp;
		bulletCount++;
	}
}

void DeleteBullet(int _ID)
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