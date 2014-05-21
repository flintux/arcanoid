#include "brick.h"

Brick* brick_create(BrickKind kind, int xPosition, int yPosition, GameMedia *media)
{
	Brick *brick = NULL;

	brick = malloc(sizeof *brick);
	if (brick == NULL)
	{
		printf("error allocating memory for brick\n");
		return brick;
	}
	brick->rect.x = xPosition;
	brick->rect.y = yPosition;
	brick->rect.w = BRICK_WIDTH;
	brick->rect.h = BRICK_HEIGHT;
	switch (kind)
	{
	case BRICK_NORMAL:
		brick->kind = BRICK_NORMAL;
		brick->live = 1;
		break;
	case BRICK_DUAL:
		brick->kind = BRICK_DUAL;
		brick->live = 2;
		break;
	case BRICK_TRIPLE:
		brick->kind = BRICK_TRIPLE;
		brick->live = 3;
		break;
	case BRICK_SUPERMAN:
		brick->kind = BRICK_SUPERMAN;
		brick->live = 9;
		break;	
	case BRICK_NONE:
	default:
		brick->kind = BRICK_NONE;
		brick->live = 0;
		break;
	}
	return brick;
}

void brick_destroy(Brick *brick)
{
	free(brick);
	brick = NULL;
}

int brick_left(Brick *brick)
{
	return brick->rect.x;
}

int brick_right(Brick *brick)
{
	return brick->rect.x + brick->rect.w;
}

int brick_top(Brick *brick)
{
	return brick->rect.y;
}

int brick_bottom(Brick *brick)
{
	return brick->rect.y + brick->rect.h;
}

int brick_height(Brick *brick)
{
	return brick->rect.h;
}

int brick_width(Brick *brick)
{
	return brick->rect.w;
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

int brick_collided(Brick *brick, Player *player)
{
	switch (brick->kind)
	{
	case BRICK_TRIPLE:
		player->score += SCORE_TRIPLE;
	case BRICK_DUAL:
		player->score += SCORE_DUAL;
	case BRICK_NORMAL:
		brick->live--;
		if (brick->live <= 0)
		{
			brick->kind = BRICK_NONE;
			player->score += SCORE_NORMAL;
			return 1;
		}
		break;
	case BRICK_SUPERMAN:
	case BRICK_NONE:
	default:
		/* do nothing :-)*/
		break;
	}
	return 0;
}

