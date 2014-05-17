#ifndef _INC_LEVEL
#define _INC_LEVEL

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "brick.h"
#include "media.h"


typedef struct Level{
	int number;
	double speedFactor;
	Brick wall[24][20];
	int rows;
	int lines;

} Level;


Level level_create_level(int number, int lines, int row, GameMedia* media);

void level_draw(Level *level, SDL_Renderer * renderer, GameMedia* media);

#endif

