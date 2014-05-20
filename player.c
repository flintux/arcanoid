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


void player_move(Player *player, Ball *ball, int ballMove, int screenWidth)
{
	if (!(player->keyTimer < SDL_GetTicks()))
	{
		return;
	}
	if (player->direction == DIRECTION_RIGHT)	//move right
	{
		if (player->playerRect.x + player->playerRect.w <= screenWidth)
		{
			player->playerRect.x += 1;
			if (!ballMove)
			{
				if (ball->ballRect.x + ball->ballRect.w / 2 < player->playerRect.x)
				{
					ball->ballRect.x += 1;
					ball->PositionX = (float)ball->ballRect.x;
				}
			}
		}
	}
	else if (player->direction == DIRECTION_LEFT) // move left
	{
		if (player->playerRect.x >= 0){
			player->playerRect.x -= 1;
			if (!ballMove)
			{
				if (ball->ballRect.x + ball->ballRect.w / 2 > player->playerRect.x + player->playerRect.w)
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