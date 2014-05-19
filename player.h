#ifndef _INC_PLAYER
#define _INC_PLAYER

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
moves player and ball if required
*/
void player_move(Player *player, Ball *ball, int ballMove, int screenWidth);

/*
places player at specified position (left upper corner of player)
*/
void player_position(Player *player, int positionX, int positionY);

#endif
