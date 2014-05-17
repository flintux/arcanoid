#ifndef _INC_PLAYER
#define _INC_PLAYER

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "ball.h"

typedef struct Ball Ball;

typedef struct Player
{
	SDL_Texture *player;
	SDL_Rect playerRect;
	int speed;
	int direction;
	unsigned int keyTimer;
} Player;


void movePlayer(Player *player, Ball *ball, int ballMove, int screenWidth);

#endif
