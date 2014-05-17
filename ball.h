#ifndef _INC_BALL
#define _INC_BALL

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "player.h"
#include "brick.h"

typedef struct Brick Brick;

typedef struct Player Player;

typedef struct Ball
{
	SDL_Texture *ball;	/*texture of the ball*/
	SDL_Rect ballRect;			/*hitbox of the ball*/
	double ballSpeedX;			/*speed of the ball*/
	double ballSpeedY;
	double PositionX;			/*position of the ball*/
	double PositionY;
	double actualSpeed;	/*default speed of ball*/
	unsigned int timer;			/*timer for SDL_Tick*/
	int updateSpeed;		/*update speed for moving ball*/
} Ball;


/*
//defines the list of possible collision
//it is based on the ball itself, so that TOP means the top of ball had a collision
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
Checks if ball touched a wall, return side of ball that touched the wall
*/
BallCollisionSide ball_check_wall_collision(Ball *ball, int borderRight, int borderBottom);


/*
Checks if ball touched player and changes ball speed accordingly.
*/
void ball_check_player_collision(Ball *ball, Player *player);


/*
Checks if ball touched brick and changes ball speed accordingly.
*/
BallCollisionSide ball_check_brick_collision(Ball *ball, Brick *brick);


#endif
