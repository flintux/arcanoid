/****************************************************************************/
/*																			*/
/*functions for ball control in arkanoid									*/
/*																			*/
/****************************************************************************/

#include "ball.h"

Ball* ball_create(GameMedia *media)
{
	Ball *ball = malloc(sizeof *ball);

	ball->ball = media->ball;
	ball->ballRect.h = BALL_SIZE;
	ball->ballRect.w = BALL_SIZE;
	ball->ballSpeedX = BALL_DEFAULT_SPEED;
	ball->ballSpeedY = -BALL_DEFAULT_SPEED;
	ball->PositionX;
	ball->PositionY;
	ball->actualSpeed = BALL_DEFAULT_SPEED;
	ball->timer = SDL_GetTicks();
	ball->updateSpeed = BALL_UPDATE_SPEED;
	return ball;
}

void ball_destroy(Ball *ball)
{
	free(ball);
	ball = NULL;
}

int ball_left(Ball *ball)
{
	return ball->ballRect.x;
}

int ball_right(Ball *ball)
{
	return ball->ballRect.x + ball->ballRect.w;
}

int ball_top(Ball *ball)
{
	return ball->ballRect.y;
}

int ball_bottom(Ball *ball)
{
	return ball->ballRect.y + ball->ballRect.h;
}

void ball_move(Ball *ball)
{
	/*move the ball
	//it would be better to take time * speed in order to avoid slow down if computer
	// doesn't process fast enough (in this case it should be fine :-)*/
	ball->PositionX += ball->ballSpeedX;
	ball->PositionY += ball->ballSpeedY;
	ball->ballRect.x = (int)ball->PositionX;
	ball->ballRect.y = (int)ball->PositionY;
}

BallCollisionSide ball_check_wall_collision(Ball *ball, int borderRight, int borderBottom)
{
	BallCollisionSide collision = COLLISION_SIDE_NONE;

	if (ball_left(ball) <= 0)
	{
		if (ball->ballSpeedX < 0)
		{
			ball->ballSpeedX = -ball->ballSpeedX;
			collision = COLLISION_SIDE_LEFT;
			puts("collision with left");
		}
	}
	else if (ball_right(ball) >= borderRight)
	{
		if (ball->ballSpeedX > 0)
		{
			ball->ballSpeedX = -ball->ballSpeedX;
			collision = COLLISION_SIDE_RIGHT;
			puts("collision with right");
		}
	}
	else if (ball_top(ball) <= 0)
	{
		if (ball->ballSpeedY < 0)
		{
			ball->ballSpeedY = -ball->ballSpeedY;
			collision = COLLISION_SIDE_TOP;
			puts("collision with top");
		}
	}
	else if (ball_bottom(ball) >= borderBottom)
	{
		collision = COLLISION_SIDE_BOTTOM;
		puts("collision with bottom");
	}
	return collision;
}

void ball_check_player_collision(Ball *ball, Player *player)
{
	if (ball_bottom(ball) >= player_top(player) &&
		ball_bottom(ball) < player_top(player) + 10 &&
		ball_right(ball) >= player_left(player) &&
		ball_left(ball) <= player_right(player))
	{
		if (ball->ballSpeedY > 0) /* only if ball is going down */
		{
			float xball;
			float xplayer;
			float shift;
			xball = (float)ball_left(ball) + (ball->ballRect.w / 2);
			xplayer = (float)player_left(player) + (player->playerRect.w / 2);
			shift = xball - xplayer;
			ball->ballSpeedX = -(float)(shift / (player->playerRect.w / 2 + ball->ballRect.w / 2))*1.2f*ball->actualSpeed;
			if (ball->ballSpeedX > 0)
			{
				ball->ballSpeedY = (float)(2.0 - ball->ballSpeedX) * ball->actualSpeed;
			}
			else
			{
				ball->ballSpeedY = (float)(2.0 + ball->ballSpeedX) * ball->actualSpeed;
			}
			if (ball->ballSpeedY > 0) ball->ballSpeedY = -ball->ballSpeedY;
			printf("speed X: %lf\tspeed y: %lf\n", ball->ballSpeedX, ball->ballSpeedY);
		}
	}
}

BallCollisionSide ball_check_brick_collision(Ball *ball, Brick *brick)
{
	BallCollisionSide collision = COLLISION_SIDE_NONE;

	if (brick->kind == BRICK_NONE)
	{
		return collision;
	}
	if (ball->ballSpeedX > 0)	/*moving right to left*/
	{
		if (ball_right(ball) == brick_left(brick) + 1) /*we just entered the brick*/
		{
			if (ball_top(ball) <= brick_bottom(brick) - 1 &&
				ball_bottom(ball) >= brick_top(brick) + 1)
			{
				ball->ballSpeedX = -ball->ballSpeedX;
				printf("ball collision on right side with brick\n");
				collision = COLLISION_SIDE_RIGHT;
			}
		}
	}
	else /*moving left to right*/
	{
		if (ball_left(ball) == brick_right(brick) - 1) /*we just entered the brick*/
		{
			if (ball_top(ball) <= brick_bottom(brick) - 1 &&
				ball_bottom(ball) >= brick_top(brick) + 1)
			{
				ball->ballSpeedX = -ball->ballSpeedX;
				printf("ball collision on left side with brick\n");
				collision = COLLISION_SIDE_LEFT;
			}
		}
	}
	if (ball->ballSpeedY > 0)	/*moving top to bottom*/
	{
		if (ball_bottom(ball) == brick_top(brick) + 1) /*we just entered the brick*/
		{
			if (ball_left(ball) <= brick_right(brick) - 1 &&
				ball_right(ball) >= brick_left(brick) + 1)
			{
				ball->ballSpeedY = -ball->ballSpeedY;
				printf("ball collision on bottom side with brick\n");
				collision = COLLISION_SIDE_BOTTOM;
			}
		}
	}
	else /*moving bottom to top*/
	{
		if (ball_top(ball) == brick_bottom(brick) - 1) /*we just entered the brick*/
		{
			if (ball_left(ball) <= brick_right(brick) - 1 &&
				ball_right(ball) >= brick_left(brick) + 1)
			{
				ball->ballSpeedY = -ball->ballSpeedY;
				printf("ball collision on top side with brick\n");
				collision = COLLISION_SIDE_TOP;
			}
		}
	}
	return collision;
}

void ball_position(Ball *ball, int positionX, int positionY)
{
	ball->ballRect.x = positionX;
	ball->PositionX = (float)positionX;
	ball->ballRect.y = positionY;
	ball->PositionY = (float)positionY;
}

