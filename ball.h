#ifndef __BALL_H__
#define __BALL_H__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "player.h"
#include "brick.h"

#define BALL_SIZE 15
#define BALL_UPDATE_SPEED 1
#define BALL_DEFAULT_SPEED -0.4f

typedef struct Brick Brick;
typedef struct Player Player;

typedef struct Ball
{
	SDL_Texture *ball;
	SDL_Rect ballRect;
	float ballSpeedX;
	float ballSpeedY;
	float PositionX;
	float PositionY;
	float actualSpeed;
	unsigned int timer;
	int updateSpeed;
} Ball;

/*
possible ball collisions
it is based on the ball itself, so that TOP means
the top of ball had a collision
*/
typedef enum BallCollisionSide
{
	COLLISION_SIDE_NONE,
	COLLISION_SIDE_TOP,
	COLLISION_SIDE_BOTTOM,
	COLLISION_SIDE_LEFT,
	COLLISION_SIDE_RIGHT
} BallCollisionSide;

/*
creates a ball with default values
*/
Ball* ball_create(GameMedia *media);

/*
destroys a ball and releases ressources
*/
void ball_destroy(Ball *ball);

/*
returns the left coordinate of the ball
*/
int ball_left(Ball *ball);

/*
returns the right coordinate of the ball
*/
int ball_right(Ball *ball);

/*
returns the top coordinate of the ball
*/
int ball_top(Ball *ball);

/*
returns the bottom coordinate of the ball
*/
int ball_bottom(Ball *ball);

/*
moves the ball
*/
void ball_move(Ball *ball);

/*
checks if ball touched a wall
returns side of ball that touched the wall
*/
BallCollisionSide ball_check_wall_collision(Ball *ball, int borderRight, int borderBottom);

/*
checks if ball touched player and changes ball speed accordingly.
*/
void ball_check_player_collision(Ball *ball, Player *player);

/*
checks if ball touched brick and changes ball speed accordingly.
*/
BallCollisionSide ball_check_brick_collision(Ball *ball, Brick *brick);

/*
places ball at given position (top left corner of ball)
*/
void ball_position(Ball *ball, int positionX, int positionY);

#endif
