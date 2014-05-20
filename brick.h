#ifndef _INC_BRICK
#define _INC_BRICK

#include <SDL2/SDL.h>
#include "media.h"

#define BRICK_HEIGHT 25
#define BRICK_WIDTH 50


typedef enum BrickKind{
	BRICK_NONE,
	BRICK_NORMAL,
	BRICK_DUAL,
	BRICK_TRIPLE,
	BRICK_SUPERMAN
}BrickKind;


typedef struct Brick{
	BrickKind kind;
	int live;
	SDL_Rect rect;
}Brick;


/*
creates a brick of special kind at given position
*/
Brick* brick_create(BrickKind kind, int xPosition, int yPosition, GameMedia *media);

/*
destroys a brick
*/
void brick_destroy(Brick *brick);

/*
returns the left coordinate of the brick
*/
int brick_left(Brick *brick);

/*
returns the right coordinate of the brick
*/
int brick_right(Brick *brick);

/*
returns the top coordinate of the brick
*/
int brick_top(Brick *brick);

/*
returns the bottom coordinate of the brick
*/
int brick_bottom(Brick *brick);

/*
draws the brick on the screen
*/
void brick_draw(Brick *brick, SDL_Renderer *renderer, GameMedia *media);

/*
updates the brick after a collision
*/
void brick_collided(Brick *brick);


#endif
