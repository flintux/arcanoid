#include "player.h"


void movePlayer(Player *player, Ball * ball, int ballMove, int screenWidth)
{
	if (player->keyTimer + player->speed < SDL_GetTicks()){
		if (player->direction == 1)	//move right
		{
			if (player->playerRect.x + player->playerRect.w <= screenWidth)
			{
				player->playerRect.x += 1;
				if (!ballMove)
				{
					if (ball->ballRect.x + ball->ballRect.w / 2 < player->playerRect.x)
					{
						ball->ballRect.x += 1;
						ball->PositionX = ball->ballRect.x;
					}
				}
			}
		}
		else if (player->direction == 2) // move left
		{
			if (player->playerRect.x >= 0){
				player->playerRect.x -= 1;
				if (!ballMove)
				{
					if (ball->ballRect.x + ball->ballRect.w / 2 > player->playerRect.x + player->playerRect.w)
					{

						ball->ballRect.x -= 1;
						ball->PositionX = ball->ballRect.x;
					}
				}
			}
		}
		player->keyTimer = SDL_GetTicks();
	}
}