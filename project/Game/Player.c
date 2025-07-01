#include "Player.h"

Player player;
sfTexture* texturePlayer;
sfTexture* textureHand;
sfTexture* shadowTexture;
sfVector2f mousePos;

sfRectangleShape* temp;

void LoadPlayer(void)
{
	//Stats
	player.speed = 500;
	player.life = 0;

	//Sprite
	player.sprite = sfSprite_create();
	player.spriteHand = sfSprite_create();
	player.spriteShadow = sfSprite_create();
	texturePlayer = sfTexture_createFromFile("Assets/Texture/Player/player.png", NULL);
	textureHand = sfTexture_createFromFile("Assets/Texture/Player/hand.png", NULL);
	shadowTexture = sfTexture_createFromFile("Assets/Texture/Player/shadow.png", NULL);

	sfSprite_setTexture(player.sprite, texturePlayer, sfTrue);
	sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, 0, 80 + 80 * IDLE, 80 + 80 * IDLE });
	sfSprite_setTexture(player.spriteHand, textureHand, sfTrue);
	sfFloatRect hitbox = sfSprite_getGlobalBounds(player.sprite);
	sfSprite_setOrigin(player.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	hitbox = sfSprite_getGlobalBounds(player.spriteHand);
	sfSprite_setOrigin(player.spriteHand, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

	LoadPlayerAnims();

	//Collider
	player.collider = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collider, (sfVector2f) { 40, 60 });
	sfRectangleShape_setOutlineThickness(player.collider, 3);
	sfRectangleShape_setOutlineColor(player.collider, sfRed);
	sfRectangleShape_setFillColor(player.collider, (sfColor) { 0, 0, 0, 0 });
	hitbox = sfRectangleShape_getGlobalBounds(player.collider);
	sfRectangleShape_setOrigin(player.collider, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });

	//Shadow
	sfSprite_setTexture(player.spriteShadow, shadowTexture, sfTrue);
	hitbox = sfSprite_getGlobalBounds(player.spriteShadow);
	sfSprite_setOrigin(player.spriteShadow, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfSprite_setPosition(player.sprite, GetSpawnPoint());
	
	//sfVector2f pos = sfSprite_getPosition(player.sprite);
	//AddEnemy(BULLET, pos, GetCurrentMap() );
}

void LoadPlayerAnims(void)
{
	player.anims = calloc(3, sizeof(Anim*));
	if (player.anims != NULL)
	{
		sfVector2u size = sfTexture_getSize(texturePlayer);
		sfIntRect first = { 0, 0, size.x / 8, size.y / 3 };
		player.anims[IDLE] = CreateAnim(texturePlayer, first, 6, 1 / 4.0f, player.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 6.f }, sfTrue);
		player.anims[IDLE]->aimOffset = (sfVector2f){ 0 };
		player.anims[IDLE]->events = malloc(sizeof(AnimEvent));

		first = (sfIntRect){ 0, 80 * 2, size.x / 8, size.y / 3 };
		player.anims[WALK] = CreateAnim(texturePlayer, first, 8, 1 / 12.0f, player.sprite, (sfVector2f) { (float)size.x / 14.f, (float)size.y / 6.f }, sfTrue);
		player.anims[WALK]->aimOffset = (sfVector2f){ 0 };
		player.anims[WALK]->events = malloc(sizeof(AnimEvent));

		size = sfTexture_getSize(textureHand);
		first = (sfIntRect){ 0, 0, size.x / 4, size.y };
		player.anims[HAND] = CreateAnim(textureHand, first, 4, 1 / 10.0f, player.spriteHand, (sfVector2f) { (float)size.x / 7.f, (float)size.y / 2.f }, sfTrue);
		player.anims[HAND]->aimOffset = (sfVector2f){ 0 };
		player.anims[HAND]->events = malloc(sizeof(AnimEvent));
	}
}

void UpdatePlayer(float _dt, sfRenderWindow* _window)
{
	sfVector2i renderMouse = sfMouse_getPositionRenderWindow(_window);
	mousePos = sfRenderWindow_mapPixelToCoords(_window, renderMouse, GetView());
	sfVector2f pos = sfSprite_getPosition(player.sprite);

	SetCameraMap(GetCurrentMap(), _window);

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

	for (int y = 0; y < GetEnemyCount(); y++)
	{
		if (IsEnemyAlive(y))
		{
			sfFloatRect enemy = GetEnemyHitBox(y);
			sfFloatRect hitbox = GetPlayerHitbox();
			if (sfFloatRect_intersects(&enemy, &hitbox, NULL))
			{
				LoseLife(1);
			}
		}
	}

	//Inv frame
	if (player.invFrame > 0 && player.invFrame < 1)
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

	//Turn frame
	if (player.turnFrame > 0)
	{
		player.turnFrame -= _dt;
	}

	//Movements
	pos = sfSprite_getPosition(player.sprite);
	sfRectangleShape_setPosition(player.collider, (sfVector2f) { pos.x + 4, pos.y + 4 });
	sfSprite_setPosition(player.spriteShadow, (sfVector2f) { pos.x + 2, pos.y + 2 });

	//Animations
	if (player.hurtFrame > 0)
	{
		player.hurtFrame -= _dt;
	}
	else 
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

void DrawPlayer(sfRenderWindow* _window, sfBool _debug)
{
	sfRenderWindow_drawSprite(_window, player.spriteShadow, NULL);
	sfRenderWindow_drawSprite(_window, player.sprite, NULL);
	sfRenderWindow_drawSprite(_window, player.spriteHand, NULL);

	if (_debug)
	{
		sfRenderWindow_drawRectangleShape(_window, player.collider, NULL);
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
	sfTexture_destroy(shadowTexture);
	shadowTexture = NULL;
  	sfRectangleShape_destroy(player.collider);
	player.collider = NULL;
}

void PlayerMove(float _dt, sfRenderWindow* _window, sfKeyCode _key)
{
	sfFloatRect hitbox = sfSprite_getGlobalBounds(player.sprite);

	switch (_key)
	{
	case sfKeyZ:
		if (!MoveCollision((sfFloatRect) { hitbox.left + 10, hitbox.top - 12, hitbox.width - 10, hitbox.height / 2 }))
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
		if (!MoveCollision((sfFloatRect) { hitbox.left, hitbox.top + 4, hitbox.width / 2, hitbox.height - 4 }))
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
		if (!MoveCollision((sfFloatRect) { hitbox.left + hitbox.width / 2 + 12, hitbox.top + 7, hitbox.width / 2, hitbox.height - 7 }))
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
	if (player.anims[HAND]->frameNum == 1 && player.fireRate)
	{
		sfVector2f pos = sfSprite_getPosition(player.spriteHand);
		if (player.anims[HAND]->frameNum == 1 && player.fireRate)
		{
			AddBullet(pos, (LookToDirection(GetMousePos(), pos) + 90), 1200, sfFalse);
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
		player.life++;
		AddLifePoint();
	}
}

void LoseLife(int _life)
{
	if (player.invFrame <= 0)
	{
		for (int i = 0; i < _life; i++)
		{
			if (player.life > 0)
				player.life--;
			DeleteLifePoint();
			player.invFrame = 1.2f;
			sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, 80, 80, 80 });
			player.hurtFrame = 0.05f;
			sfSprite_setColor(player.spriteHand, (sfColor) { 255, 255, 255, 0 });
		}
	}
}

void PlayerTransition(sfRenderWindow* _window)
{
	if (sfKeyboard_isKeyPressed(sfKeyS))
		sfSprite_move(player.sprite, (sfVector2f) { 0, 100 });
	if (sfKeyboard_isKeyPressed(sfKeyZ))
		sfSprite_move(player.sprite, (sfVector2f) { 0, -100	});
	if (sfKeyboard_isKeyPressed(sfKeyD))
		sfSprite_move(player.sprite, (sfVector2f) { 100, 0 });
	if (sfKeyboard_isKeyPressed(sfKeyQ))
		sfSprite_move(player.sprite, (sfVector2f) { -100, 0 });
}

int GetHP(void)
{
	return player.life;
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
	return mousePos;
}
