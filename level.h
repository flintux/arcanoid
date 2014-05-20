#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "brick.h"
#include "media.h"

#define LEVEL_ROW_MAX 20
#define LEVEL_LINE_MAX 24
#define LEVEL_SPEED_FACTOR 1
#define LEVEL_NAME_BASE "levels/level_%02d.txt"
#define LEVEL_MAX 10

typedef struct Level{
	int number;
	float speedFactor;
	Brick *wall[LEVEL_LINE_MAX][LEVEL_ROW_MAX];
	int rows;
	int lines;
	int destroyableBricks;
} Level;

/*
creates a random level
*/
Level* level_create_random(int number, int lines, int row, GameMedia* media);

/*
destroys level
*/
void level_destroy(Level *level);

/*
draws the bricks on the screen
*/
void level_draw(Level *level, SDL_Renderer * renderer, GameMedia* media);

/*
load a level from a file
*/
Level* level_load_file(char *path, SDL_Renderer *renderer, GameMedia* media);

/*
counts destroyable bricks in level
*/
int level_count_bricks(Level *level);

/*
loads first level
*/
Level* level_first(SDL_Renderer *renderer, GameMedia* media);

/*
loads next level according to actual level
*/
Level* level_next(Level *level, SDL_Renderer *renderer, GameMedia* media);

#endif

