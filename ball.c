/****************************************************************************/
/*																			*/
/*functions for ball control in arkanoid									*/
/*																			*/
/****************************************************************************/

#include "ball.h"

BallCollisionSide ball_check_wall_collision(Ball *ball, int borderRight, int borderBottom)
{
	BallCollisionSide collision = COLLISION_SIDE_NONE;
	/*int window_h;
	int window_w;
	SDL_GetWindowSize(window, &window_w, &window_h);
	printf("window size is %d x %d", window_w, window_h);*/
	if (ball->ballRect.x <= 0) // left border
	{
		if (ball->ballSpeedX < 0)
		{
			ball->ballSpeedX = -ball->ballSpeedX;
			collision = COLLISION_SIDE_LEFT;
		}
	}
	if (ball->ballRect.x + ball->ballRect.w >= borderRight)  //right border
	{
		if (ball->ballSpeedX > 0)
		{
			ball->ballSpeedX = -ball->ballSpeedX;
			collision = COLLISION_SIDE_RIGHT;
		}
	}
	if (ball->ballRect.y <= 0) // top border
	{
		if (ball->ballSpeedY < 0)
		{
			ball->ballSpeedY = -ball->ballSpeedY;
			collision = COLLISION_SIDE_TOP;
		}
	}
	if (ball->ballRect.y + ball->ballRect.h >= borderBottom)	// bottom border = you lost :-(
	{
		collision = COLLISION_SIDE_BOTTOM;
	}

	return collision;
}

void ball_check_player_collision(Ball *ball, Player *player)
{

	if ((ball->ballRect.y + ball->ballRect.h >= player->playerRect.y) && (ball->ballRect.y + ball->ballRect.h < player->playerRect.y + 10) && ((ball->ballRect.x + ball->ballRect.w >= player->playerRect.x) && (ball->ballRect.x <= player->playerRect.x + player->playerRect.w)))
	{
		if (ball->ballSpeedY > 0){	//only update if ball is going down

			double xball;
			double xplayer;
			double shift;
			
			xball = ball->ballRect.x + (ball->ballRect.w / 2);
			xplayer = player->playerRect.x + (player->playerRect.w / 2);

			shift = xball - xplayer;

			ball->ballSpeedX = -(double)(shift / (player->playerRect.w / 2 + ball->ballRect.w / 2))*1.2f*ball->actualSpeed;
			ball->ballSpeedY = (double)(2 - abs(ball->ballSpeedX))*ball->actualSpeed;
			if (ball->ballSpeedY > 0) ball->ballSpeedY = -ball->ballSpeedY;
			printf("speed X: %lf\tspeed y: %lf\n", ball->ballSpeedX, ball->ballSpeedY);
			//printf("\a");
		}
	}
}

BallCollisionSide ball_check_brick_collision(Ball *ball, Brick *brick)
{
	BallCollisionSide collision = COLLISION_SIDE_NONE;
	/*int window_h;
	int window_w;
	SDL_GetWindowSize(window, &window_w, &window_h);
	printf("window size is %d x %d", window_w, window_h);*/

	/*we only check the point that could it according to speed*/

	if (ball->ballSpeedX > 0)	/*moving right to left*/
	{
		if (ball->ballRect.x + ball->ballRect.w == brick->rect.x + 1) /*we just entered the brick*/
		{
			if ((ball->ballRect.y <= (brick->rect.y + brick->rect.h))
				&& ((ball->ballRect.y + ball->ballRect.h) >= brick->rect.y ))
			{
				ball->ballSpeedX = -ball->ballSpeedX;
				printf("ball collision on right side with brick\n");
				collision = COLLISION_SIDE_RIGHT;
			}
		}
	}
	else /*moving left to right*/
	{
		if (ball->ballRect.x == brick->rect.x + brick->rect.w -1) /*we just entered the brick*/
		{
			if ((ball->ballRect.y <= (brick->rect.y + brick->rect.h))
				&& ((ball->ballRect.y + ball->ballRect.h) >= brick->rect.y))
			{
				ball->ballSpeedX = -ball->ballSpeedX;
				printf("ball collision on left side with brick\n");
				collision = COLLISION_SIDE_LEFT;
			}
		}
	}

	if (ball->ballSpeedY > 0)	/*moving top to bottom*/
	{
		if (ball->ballRect.y + ball->ballRect.h == brick->rect.y + 1) /*we just entered the brick*/
		{
			if ((ball->ballRect.x <= (brick->rect.x + brick->rect.w))
				&& ((ball->ballRect.x + ball->ballRect.w) >= brick->rect.x))
			{
				ball->ballSpeedY = -ball->ballSpeedY;
				printf("ball collision on bottom side with brick\n");
				collision = COLLISION_SIDE_BOTTOM;
			}
		}
	}
	else /*moving bottom to top*/
	{
		if (ball->ballRect.y == brick->rect.y + brick->rect.h -1) /*we just entered the brick*/
		{
			if ((ball->ballRect.x <= (brick->rect.x + brick->rect.w))
				&& ((ball->ballRect.x + ball->ballRect.w) >= brick->rect.x))
			{
				ball->ballSpeedY = -ball->ballSpeedY;
				printf("ball collision on top side with brick\n");
				collision = COLLISION_SIDE_TOP;
			}
		}
	}


	return collision;

}

