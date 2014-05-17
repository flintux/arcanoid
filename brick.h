#ifndef _INC_BRICK
#define _INC_BRICK

#include <SDL2/SDL.h>
#include "media.h"


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
Creates a brick of special kind at given position
*/
Brick brick_create(BrickKind kind, int xPosition, int yPosition, GameMedia *media);

/*
Draws the brick on the screen
*/
void brick_draw(Brick *brick, SDL_Renderer *renderer, GameMedia *media);

void brick_collided(Brick *brick);


#endif
