#include "brick.h"


Brick brick_create(BrickKind kind, int xPosition, int yPosition, GameMedia *media)
{
	Brick newBrick;
	switch (kind)
	{
	case BRICK_NORMAL:
		newBrick.kind = BRICK_NORMAL;
		newBrick.live = 1;
		newBrick.rect.x = xPosition;
		newBrick.rect.y = yPosition;
		newBrick.rect.w = 50;
		newBrick.rect.h = 25;
		break;
	case BRICK_DUAL:
		newBrick.kind = BRICK_DUAL;
		newBrick.live = 2;
		newBrick.rect.x = xPosition;
		newBrick.rect.y = yPosition;
		newBrick.rect.w = 50;
		newBrick.rect.h = 25;
		break;
	case BRICK_TRIPLE:
		newBrick.kind = BRICK_TRIPLE;
		newBrick.live = 3;
		newBrick.rect.x = xPosition;
		newBrick.rect.y = yPosition;
		newBrick.rect.w = 50;
		newBrick.rect.h = 25;
		break;
	case BRICK_SUPERMAN:
		newBrick.kind = BRICK_SUPERMAN;
		newBrick.live = 0;
		newBrick.rect.x = xPosition;
		newBrick.rect.y = yPosition;
		newBrick.rect.w = 50;
		newBrick.rect.h = 25;
		break;	
	case BRICK_NONE:
	default:
		newBrick.kind = BRICK_NONE;
		newBrick.live = 0;
		newBrick.rect.x = xPosition;
		newBrick.rect.y = yPosition;
		newBrick.rect.w = 50;
		newBrick.rect.h = 25;
		break;
	}

	return newBrick;

}


void brick_draw(Brick *brick, SDL_Renderer *renderer, GameMedia *media)
{
	switch (brick->kind)
	{
	case BRICK_NORMAL:
		SDL_RenderCopy(renderer, media->brickNormal, NULL, &brick->rect);
		break;
	case BRICK_DUAL:
		switch (brick->live){
		case 1:
			SDL_RenderCopy(renderer, media->brickDualOne, NULL, &brick->rect);
			break;
		case 2:
		default:
			SDL_RenderCopy(renderer, media->brickDualTwo, NULL, &brick->rect);
			break;
		}
		break;
	case BRICK_TRIPLE:
		switch (brick->live){
		case 1:
			SDL_RenderCopy(renderer, media->brickTripleOne, NULL, &brick->rect);
			break;
		case 2:
			SDL_RenderCopy(renderer, media->brickTripleTwo, NULL, &brick->rect);
			break;
		case 3:
		default:
			SDL_RenderCopy(renderer, media->brickTripleThree, NULL, &brick->rect);
			break;
		}
		break;
	case BRICK_SUPERMAN:
		SDL_RenderCopy(renderer, media->brickSuperman, NULL, &brick->rect);
		break;
	case BRICK_NONE:
	default:
		/* do nothing :-)*/
		break;
	}

}

void brick_collided(Brick *brick)
{
	switch (brick->kind)
	{
	case BRICK_NORMAL:
	case BRICK_DUAL:
	case BRICK_TRIPLE:
		brick->live--;
		if (brick->live <= 0)
		{
			brick->kind = BRICK_NONE;
		}
		break;
	case BRICK_SUPERMAN:
	case BRICK_NONE:
	default:
		/* do nothing :-)*/
		break;
	}
}

