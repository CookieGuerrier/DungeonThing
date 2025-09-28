#include "Player.h"

Player player;
sfSprite* rope;
sfTexture* texturePlayer;
sfTexture* textureHand;
sfTexture* textureShadow;
sfTexture* textureRope;
sfVector2f mousePosPlayer;

int artifactCount;
ShopType artifact[9];
sfBool effects[9];

float chargeTimer;
float rollTimer;
float rollCooldown;
sfVector2f enemyPos;
sfBool ropeActive;

float startTimer;

sfRectangleShape* temp;
sfBool deb;

void LoadPlayer(void)
{
	//Stats
	player.speed = 500;
	player.life = 0;
	artifactCount = 0;
	startTimer = 1;

	//Sprite
	player.sprite = sfSprite_create();
	player.spriteHand = sfSprite_create();
	player.spriteShadow = sfSprite_create();
	rope = sfSprite_create();
	texturePlayer = sfTexture_createFromFile("Assets/Texture/Player/player.png", NULL);
	textureHand = sfTexture_createFromFile("Assets/Texture/Player/hand.png", NULL);
	textureShadow = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);
	textureRope = sfTexture_createFromFile("Assets/Texture/Player/rope.png", NULL);

	sfSprite_setTexture(player.sprite, texturePlayer, sfTrue);
	sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, 0, 80 + 80 * IDLE, 80 + 80 * IDLE });
	sfSprite_setTexture(player.spriteHand, textureHand, sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(player.sprite);
	sfSprite_setOrigin(player.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	hitbox = sfSprite_getGlobalBounds(player.spriteHand);
	sfSprite_setOrigin(player.spriteHand, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

	//Rope
	sfSprite_setTexture(rope, textureRope, sfTrue);
	hitbox = sfSprite_getGlobalBounds(rope);
	sfSprite_setOrigin(rope, (sfVector2f) { hitbox.left, hitbox.height / 2 });
	LoadPlayerAnims();

	for (int i = 0; i < 9; i++)
	{
		effects[i] = sfFalse;
	}
	//Collider
	player.collider = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collider, (sfVector2f) { 40, 40 });
	sfRectangleShape_setOutlineThickness(player.collider, 3);
	sfRectangleShape_setOutlineColor(player.collider, sfRed);
	sfRectangleShape_setFillColor(player.collider, (sfColor) { 0, 0, 0, 0 });
	hitbox = sfRectangleShape_getGlobalBounds(player.collider);
	sfRectangleShape_setOrigin(player.collider, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

	//Shadow
	sfSprite_setTexture(player.spriteShadow, textureShadow, sfTrue);
	hitbox = sfSprite_getGlobalBounds(player.spriteShadow);
	sfSprite_setOrigin(player.spriteShadow, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfSprite_setPosition(player.sprite, GetSpawnPoint());

	player.anims[ROLL]->frameNum = player.anims[ROLL]->frameMax;

	player.stepSound = GetSoundCount();
	AddSound(S_STEP);
	player.shootSound = GetSoundCount();
	AddSound(S_SHOOT);
	player.hurtSound= GetSoundCount();
	AddSound(S_HURT);	
	player.dashSound= GetSoundCount();
	AddSound(S_DASH);
}

void LoadPlayerAnims(void)
{
	player.anims = calloc(5, sizeof(Anim*));
	if (player.anims != NULL)
	{
		sfVector2u size = sfTexture_getSize(texturePlayer);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 5 };
		player.anims[IDLE] = CreateAnim(texturePlayer, first, 6, 1 / 4.0f, player.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 10.f }, sfTrue);
		player.anims[IDLE]->aimOffset = (sfVector2f){ 0 };
		player.anims[IDLE]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 80 * 2, size.x / 8, size.y / 5 };
		player.anims[WALK] = CreateAnim(texturePlayer, first, 8, 1 / 12.0f, player.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 10.f }, sfTrue);
		player.anims[WALK]->aimOffset = (sfVector2f){ 0 };
		player.anims[WALK]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 80 * 3, size.x / 8, size.y / 5 };
		player.anims[ROLL] = CreateAnim(texturePlayer, first, 8, 1 / 16.0f, player.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 10.f }, sfFalse);
		player.anims[ROLL]->aimOffset = (sfVector2f){ 0 };
		player.anims[ROLL]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 80 * 4, size.x / 8, size.y / 5 };
		player.anims[DEATH] = CreateAnim(texturePlayer, first, 8, 1 / 8.0f, player.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 10.f }, sfFalse);
		player.anims[DEATH]->aimOffset = (sfVector2f){ 0 };
		player.anims[DEATH]->events = malloc(sizeof(AnimEvent));

		size = sfTexture_getSize(textureHand);
		first = (sfIntRect){ 0, 0, size.x / 4, size.y };
		player.anims[HAND] = CreateAnim(textureHand, first, 4, 1 / 8.0f, player.spriteHand, (sfVector2f) { (float)size.x / 7.f, (float)size.y / 2.f }, sfTrue);
		player.anims[HAND]->aimOffset = (sfVector2f){ 0 };
		player.anims[HAND]->events = malloc(sizeof(AnimEvent));
	}
}

void UpdatePlayer(float _dt, sfRenderWindow* _window)
{
	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	mousePosPlayer = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetView());
	sfVector2f pos = sfSprite_getPosition(player.sprite);
	sfFloatRect hitbox = GetPlayerHitbox();

	if (player.life > 0)
	{
		if (startTimer < 0)
		{
			//Move
			if (sfKeyboard_isKeyPressed(sfKeyZ) && !sfKeyboard_isKeyPressed(sfKeyS))
			{
				PlayerMove(_dt, _window, sfKeyZ);
			}
			else if (sfKeyboard_isKeyPressed(sfKeyS) && !sfKeyboard_isKeyPressed(sfKeyZ))
			{
				PlayerMove(_dt, _window, sfKeyS);
			}
			else
			{
				player.velocity.y = 0;
			}
			if (sfKeyboard_isKeyPressed(sfKeyQ) && !sfKeyboard_isKeyPressed(sfKeyD))
			{
				PlayerMove(_dt, _window, sfKeyQ);
			}
			else if (sfKeyboard_isKeyPressed(sfKeyD) && !sfKeyboard_isKeyPressed(sfKeyQ))
			{
				PlayerMove(_dt, _window, sfKeyD);
			}
			else
			{
				player.velocity.x = 0;
			}

			sfSprite_move(player.sprite, (sfVector2f) { player.velocity.x, player.velocity.y });

			//Function
			PlayerShoot(_dt);
			HandThing(pos);

			//Enemy Collision
			for (int y = 0; y < GetEnemyCount(); y++)
			{
				if (IsEnemyAlive(y))
				{
					sfFloatRect enemy = GetEnemyHitBox(y);
					if (sfFloatRect_intersects(&enemy, &hitbox, NULL) && rollTimer <= 0.3f)
					{
						LoseLife(1);
					}
				}
			}

			//Boss collision
			sfFloatRect boss = GetBossHitbox();
			if (sfFloatRect_intersects(&boss, &hitbox, NULL) && rollTimer <= 0.3f && !GetBossDead())
			{
				LoseLife(1);
			}

			//Inv frame
			if (player.invFrame >= 0 && player.invFrame < 1)
			{
				player.invFrame -= _dt;
				if ((int)(player.invFrame * 10) % 2 == 0)
				{
					sfSprite_setColor(player.sprite, (sfColor) { 255, 255, 255, 255 });
					sfSprite_setColor(player.spriteHand, (sfColor) { 255, 255, 255, 255 });
				}
				else
				{
					sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, 0, 80 + 80 * IDLE, 80 + 80 * IDLE });
					sfSprite_setColor(player.sprite, (sfColor) { 255, 255, 255, 0 });
					sfSprite_setColor(player.spriteHand, (sfColor) { 255, 255, 255, 0 });
				}
			}
			else if (player.invFrame > 1)
			{
				player.invFrame -= _dt;
			}

			//Effect
			{
				if (GetEffect(STAMP))
				{
					if (player.invFrame < 0)
					{
						player.anims[HAND]->rate = 1 / 8.0f;
					}
					else
					{
						player.anims[HAND]->rate = 1 / 32.0f;
					}
				}
			}

			//Turn frame
			if (player.turnFrame > 0)
			{
				player.turnFrame -= _dt;
			}

			if (rollTimer < 0)
			{
				player.speed = 500;
			}
			else
			{
				rollTimer -= _dt;
				player.speed -= 20;
			}

			//Roll
			if (rollTimer > 0)
			{
				rollTimer -= _dt;
				player.speed -= 10;
			}
			else
			{
				player.speed = 500;
			}
			if (rollCooldown > 0)
			{
				rollCooldown -= _dt;
			}

			//Animations
			if (player.hurtFrame > 0)
			{
				player.hurtFrame -= _dt;
			}
			else
			{
				if (rollTimer < 0 && IsFinishedAnim(player.anims[ROLL]))
				{
					if (player.velocity.x == 0 && player.velocity.y == 0)
					{
						UpdateAnim(_dt, player.anims[IDLE]);
					}
					else
					{
						UpdateAnim(_dt, player.anims[WALK]);
					}
				}
				else
				{
					UpdateAnim(_dt, player.anims[ROLL]);
				}
			}

			if (GetEffect(BATTERY))
			{
				sfSprite_setTextureRect(player.spriteHand, (sfIntRect) { 0, 0, 48, 38 });
				sfSprite_setColor(player.spriteHand, (sfColor) { 255, 255 - (int)(chargeTimer * 200), 255 - (int)(chargeTimer * 200), 255 });
				if (sfMouse_isButtonPressed(sfMouseLeft) && chargeTimer < 1)
				{
					player.anims[HAND]->frameNum = 0;
					if (GetEffect(STAMP) && player.invFrame > 0)
					{
						chargeTimer += _dt * 12;
					}
					else
					{
						chargeTimer += _dt * 3;
					}
				}
				else if (!sfMouse_isButtonPressed(sfMouseLeft) && chargeTimer >= 1)
				{
					UpdateAnim(_dt, player.anims[HAND]);
					player.turnFrame = 0.3f;
					if (player.anims[HAND]->frameNum == 1)
					{
						chargeTimer = 0;
					}
				}
				else if (!sfMouse_isButtonPressed(sfMouseLeft))
				{
					player.anims[HAND]->frameNum = 0;
					chargeTimer = 0;
				}
			}
			else
			{
				if (sfMouse_isButtonPressed(sfMouseLeft) || player.anims[HAND]->frameNum != 0)
				{
					UpdateAnim(_dt, player.anims[HAND]);
					player.turnFrame = 0.3f;
				}
				if (sfMouse_isButtonPressed(sfMouseLeft) && player.anims[HAND]->frameNum == 0)
				{
					player.anims[HAND]->frameNum = 1;
				}
			}

			//Rope
			if (GetEffect(COWBOY_HAT))
			{
				sfSprite_setPosition(rope, pos);
				sfSprite_setRotation(rope, LookToDirection(enemyPos, pos));
				if (GetDistanceVector2f(enemyPos, pos) < 300 && ropeActive)
				{
					sfSprite_setTextureRect(rope, (sfIntRect) { 0, 0, (int)GetDistanceVector2f(enemyPos, pos), 10 });
				}
				else
				{
					ropeActive = sfFalse;
					sfSprite_setTextureRect(rope, (sfIntRect) { 0, 0, 0, 10 });
					RopeEnemy(9999);
				}
			}

			//Sound
			if ((player.velocity.x != 0 || player.velocity.y != 0) && rollTimer < 0)
			{
				if (player.stepTimer <= 0)
				{
					PlaySound(player.stepSound);
					player.stepTimer = 0.3f;
				}
				else
				{
					player.stepTimer -= _dt;
				}
			}
			else
			{
				player.stepTimer = 0.2f;
			}
		}
		else
		{
			startTimer -= _dt;
		}
	}
	else
	{
		UpdateAnim(_dt, player.anims[DEATH]);
	}

	//Movements
	pos = sfSprite_getPosition(player.sprite);
	sfRectangleShape_setPosition(player.collider, (sfVector2f) { pos.x, pos.y + 5 });
	sfSprite_setPosition(player.spriteShadow, (sfVector2f) { pos.x, pos.y });
}

void DrawPlayer(sfRenderWindow* _window, sfBool _debug)
{
	sfRenderWindow_drawSprite(_window, player.spriteShadow, NULL);
	sfRenderWindow_drawSprite(_window, player.sprite, NULL);
	deb = _debug;
	if (rollTimer <= 0 && startTimer <= 0)
	{
		sfRenderWindow_drawSprite(_window, player.spriteHand, NULL);
	}
}

void DrawRope(sfRenderWindow* _window)
{
	if (GetEffect(COWBOY_HAT))
	{
		sfRenderWindow_drawSprite(_window, rope, NULL);
	}
}

void CleanupPlayer(void)
{
	sfSprite_destroy(player.sprite);
	player.sprite = NULL;
	sfTexture_destroy(texturePlayer);
	texturePlayer = NULL;
	sfSprite_destroy(player.spriteHand);
	player.spriteHand = NULL;
	sfTexture_destroy(textureHand);
	textureHand = NULL;
	sfSprite_destroy(player.spriteShadow);
	player.spriteShadow = NULL;
	sfTexture_destroy(textureShadow);
	textureShadow = NULL;
	sfSprite_destroy(rope);
	rope = NULL;
	sfTexture_destroy(textureRope);
	textureRope = NULL;
	sfRectangleShape_destroy(player.collider);
	player.collider = NULL;

	DeleteSound(player.stepSound);
	DeleteSound(player.shootSound);
	DeleteSound(player.hurtSound);
	DeleteSound(player.dashSound);
}

void PlayerMove(float _dt, sfRenderWindow* _window, sfKeyCode _key)
{
	sfFloatRect hitbox = sfSprite_getGlobalBounds(player.sprite);

	switch (_key)
	{
	case sfKeyZ:
		if (!MoveCollision((sfFloatRect) { hitbox.left + 10, hitbox.top + 30, hitbox.width - 10, hitbox.height / 2 }))
		{
			player.velocity.y = -player.speed * _dt;
		}
		else
		{
			player.velocity.y = 0;
		}
		break;
	case sfKeyS:
		if (!MoveCollision((sfFloatRect) { hitbox.left + 11, hitbox.top + hitbox.height / 2 + 10, hitbox.width - 11, hitbox.height / 2 }))
		{
			player.velocity.y = player.speed * _dt;
		}
		else
		{
			player.velocity.y = 0;
		}
		break;
	case sfKeyQ:
		if (!MoveCollision((sfFloatRect) { hitbox.left, hitbox.top + 40, hitbox.width / 2, hitbox.height - 40 }))
		{
			player.velocity.x = -player.speed * _dt;
		}
		else
		{
			player.velocity.x = 0;
		}
		if (player.turnFrame <= 0)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { -1, 1 });
		}
		break;
	case sfKeyD:
		if (!MoveCollision((sfFloatRect) { hitbox.left + hitbox.width / 2 + 12, hitbox.top + 42, hitbox.width / 2, hitbox.height - 42 }))
		{
			player.velocity.x = player.speed * _dt;
		}
		else
		{
			player.velocity.x = 0;
		}
		if (player.turnFrame <= 0)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { 1, 1 });
		}
		break;
	default:
		break;
	}

	if (player.velocity.x != 0 && player.velocity.y != 0)
	{
		player.velocity.x *= 0.8f;
		player.velocity.y *= 0.8f;
	}
}

void PlayerShoot(float _dt)
{
	if (player.anims[HAND]->frameNum == 1 && player.fireRate && rollTimer <= 0)
	{
		sfVector2f pos = sfSprite_getPosition(player.spriteHand);
		if (player.anims[HAND]->frameNum == 1 && player.fireRate)
		{
			PlaySound(player.shootSound);
			//Tha shoot
			AddBullet(pos, (LookToDirection(GetMousePos(), pos) + 90), 1000, sfFalse, sfFalse);
			if (effects[SAW])
			{
				AddBullet(pos, (LookToDirection(GetMousePos(), pos) + 120), 1000, sfFalse, sfFalse);
				AddBullet(pos, (LookToDirection(GetMousePos(), pos) + 70), 1000, sfFalse, sfFalse);
			}
		}
		player.turnFrame = 0.3f;
		if (abs((int)(LookToDirection(GetMousePos(), pos))) < 90)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { 1, 1 });
		}
		else
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { -1, 1 });
		}
		player.fireRate = sfFalse;
	}
	else if (player.anims[HAND]->frameNum != 1)
	{
		player.fireRate = sfTrue;
	}
}

void PlayerRoll(void)
{
	if (rollTimer <= 0 && rollCooldown <= 0 && startTimer <= 0 && (player.velocity.y != 0 || player.velocity.x != 0))
	{
		rollTimer = 1;
		rollCooldown = 0.5f;
		player.speed = 1000;
		PlaySound(player.dashSound);
		ResetAnim(player.anims[ROLL]);
	}
}

void HandThing(sfVector2f _pos)
{
	if (sfSprite_getScale(player.sprite).x == 1)
	{
		sfSprite_setPosition(player.spriteHand, (sfVector2f) { _pos.x + 40, _pos.y + 20 });
		sfSprite_setScale(player.spriteHand, (sfVector2f) { 1, 1 });
	}
	else
	{
		sfSprite_setPosition(player.spriteHand, (sfVector2f) { _pos.x - 40, _pos.y + 20 });
		sfSprite_setScale(player.spriteHand, (sfVector2f) { -1, 1 });
	}
}

void GainLife(int _life)
{
	for (int i = 0; i < _life; i++)
	{
		if (player.life < 6)
		{
			player.life++;
			AddLife();
		}
	}
}

void LoseGold(int _gold)
{
	for (int i = 0; i < _gold; i++)
	{
		if (player.gold > 0)
		{
			player.gold--;
			UpdateGold(player.gold);
		}
	}
}

void GainGold(int _gold)
{
	player.gold += _gold;
	UpdateGold(player.gold);
}

void SetArtifact(int _artifact)
{
	artifact[artifactCount] = _artifact;
	effects[_artifact] = sfTrue;
	artifactCount++;
}

void SetRope(sfVector2f _dis)
{
	enemyPos = _dis;
	ropeActive = sfTrue;
}

void LoseLife(int _life)
{
	if (player.invFrame <= 0 && deb == sfFalse)
	{
		for (int i = 0; i < _life; i++)
		{
			if (player.life > 0)
			{
				player.life--;
				DeleteLife();
				PlaySound(player.hurtSound);
			}
			player.invFrame = 1.2f;
			sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, 80, 80, 80 });
			player.hurtFrame = 0.05f;
			sfSprite_setColor(player.spriteHand, (sfColor) { 255, 255, 255, 0 });
			SetDamageCamera();
		}
	}
}

void PlayerTransition(int _num)
{
	switch (_num)
	{
	case 0:
		sfSprite_move(player.sprite, (sfVector2f) { 0, 100 });
		break;
	case 1:
		sfSprite_move(player.sprite, (sfVector2f) { 0, -100 });
		break;
	case 2:
		sfSprite_move(player.sprite, (sfVector2f) { 100, 0 });
		break;
	case 3:
		sfSprite_move(player.sprite, (sfVector2f) { -100, 0 });
		break;
	default:
		break;
	}
}

void SetPlayerPosition(void)
{
	sfSprite_setPosition(player.sprite, GetSpawnPoint());
	startTimer = 1;
}

int GetHP(void)
{
	return player.life;
}

int GetMoney(void)
{
	return player.gold;
}

sfVector2f GetPlayerPos(void)
{
	return sfSprite_getPosition(player.sprite);
}

sfFloatRect GetPlayerHitbox(void)
{
	return sfRectangleShape_getGlobalBounds(player.collider);
}

sfVector2f GetMousePos(void)
{
	return mousePosPlayer;
}

int GetArtifactCount(void)
{
	return artifactCount;
}

sfBool GetEffect(int _artifact)
{
	return effects[_artifact];
}

float GetRoll(void)
{
	return rollTimer;
}

sfBool DeathFinished(void)
{
	return IsFinishedAnim(player.anims[DEATH]);
}

void ColorPlayer(sfBool _isTrue)
{
	if (_isTrue)
	{
		sfSprite_setColor(player.sprite, (sfColor) { 255, 200,255,255 });
	}
	else
	{
		sfSprite_setColor(player.sprite, (sfColor) { 255, 255, 255, 255 });
	}
}