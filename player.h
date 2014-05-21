#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "media.h"
#include "ball.h"

#define PLAYER_HEIGHT 20
#define PLAYER_WIDTH 80
#define PLAYER_DEFAULT_LIFES 3
#define PLAYER_DEFAULT_SPEED 2

typedef struct Ball Ball;

typedef enum PlayerDirection{
	DIRECTION_NONE,
	DIRECTION_RIGHT,
	DIRECTION_LEFT
}PlayerDirection;

typedef struct Player
{
	SDL_Texture *sprite;
	SDL_Rect playerRect;
	int speed;
	PlayerDirection direction;
	unsigned int keyTimer;
	int lifes;
	int score;
} Player;

/*
creates a player
*/
Player* player_create(GameMedia *media);

/*
destroys a player
*/
void player_destroy(Player *player);

/*
returns the left coordinate of the player
*/
int player_left(Player *player);

/*
returns the right coordinate of the player
*/
int player_right(Player *player);

/*
returns the top coordinate of the player
*/
int player_top(Player *player);

/*
returns the bottom coordinate of the player
*/
int player_bottom(Player *player);

/*
returns the height of the player
*/
int player_height(Player *player);

/*
returns the width of the player
*/
int player_width(Player *player);

/*
return player X center coordinate
*/
int player_center_x(Player *player);

/*
moves player and ball if required
*/
void player_move(Player *player, Ball *ball, int ballMove, int screenWidth);

/*
places player at specified position (left upper corner of player)
*/
void player_position(Player *player, int positionX, int positionY);

#endif
