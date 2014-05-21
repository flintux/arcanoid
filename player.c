#include "player.h"

Player* player_create(GameMedia *media)
{
	Player *player = NULL;

	player = malloc(sizeof *player);
	if (player == NULL)
	{
		printf("error allocating memory for player\n");
		return player;
	}
	player->sprite = media->player;
	player->playerRect.x = 0;
	player->playerRect.y = 0;
	player->playerRect.w = PLAYER_WIDTH;
	player->playerRect.h = PLAYER_HEIGHT;
	player->speed = PLAYER_DEFAULT_SPEED;
	player->keyTimer = SDL_GetTicks();
	player->direction = DIRECTION_NONE;
	player->lifes = PLAYER_DEFAULT_LIFES;
	player->score = 0;
	return player;
}

void player_destroy(Player *player)
{
	if (player != NULL)
	{
		free(player);
		player = NULL;
	}
}

int player_left(Player *player)
{
	return player->playerRect.x;
}

int player_right(Player *player)
{
	return player->playerRect.x + player->playerRect.w;
}

int player_top(Player *player)
{
	return player->playerRect.y;
}

int player_bottom(Player *player)
{
	return player->playerRect.y + player->playerRect.h;
}

void player_move(Player *player, Ball *ball, int ballMove, int screenWidth)
{
	if (!(player->keyTimer < SDL_GetTicks()))
	{
		return;
	}
	if (player->direction == DIRECTION_RIGHT)
	{
		if (player_right(player) <= screenWidth)
		{
			player->playerRect.x += 1;
			if (!ballMove)
			{
				if (ball->ballRect.x + ball->ballRect.w / 2 < player_left(player))
				{
					ball->ballRect.x += 1;
					ball->PositionX = (float)ball->ballRect.x;
				}
			}
		}
	}
	else if (player->direction == DIRECTION_LEFT) // move left
	{
		if (player_left(player) >= 0)
		{
			player->playerRect.x -= 1;
			if (!ballMove)
			{
				if (ball->ballRect.x + ball->ballRect.w / 2 > player_right(player))
				{
					ball->ballRect.x -= 1;
					ball->PositionX = (float)ball->ballRect.x;
				}
			}
		}
	}
	/*set timer for next time*/
	player->keyTimer = SDL_GetTicks() + player->speed;
}

void player_position(Player *player, int positionX, int positionY)
{
	player->playerRect.x = positionX;
	player->playerRect.y = positionY;
}