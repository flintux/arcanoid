#include "level.h"




Level* level_create_random(int number, int lines, int rows, GameMedia* media)
{
	//srand(time(NULL));
	srand(SDL_GetTicks());
	BrickKind kind;
	Level *level = NULL;
	level = (Level *)malloc(sizeof(Level));
	if (level == NULL)
	{
		return level;
	}
	int line;
	int row;
	level->number = number;
	if (rows > LEVEL_ROW_MAX) rows = LEVEL_ROW_MAX;
	if (lines > LEVEL_LINE_MAX) lines = LEVEL_LINE_MAX;
	level->lines = lines;
	level->rows = rows;
	level->speedFactor = 1;

	for (line = 0; line < lines; line++)
	{
		for (row = 0; row < rows; row++)
		{
			switch (rand() % 5)
			{
			case 0:
				kind = BRICK_NONE;
				break;
			case 1:
				kind = BRICK_NORMAL;
				break;
			case 2:
				kind = BRICK_DUAL;
				break;
			case 3:
				kind = BRICK_TRIPLE;
				break;
			case 4:
				kind = BRICK_SUPERMAN;
				break;
			}
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
		}
	}
	return level;
}


void level_destroy(Level *level)
{
	int line;
	int row;

	if (level == NULL)
	{
		return;
	}
	for (line = 0; line < level->lines; line++)
	{
		for (row = 0; row < level->rows; row++)
		{
			if (level->wall[line][row] != NULL)
			{
				brick_destroy(level->wall[line][row]);
			}
		}
	}

	free(level);
	level = NULL;
}



void level_draw(Level *level, SDL_Renderer *renderer, GameMedia* media)
{
	int line;
	int row;
	for (line = 0; line < level->lines; line++)
	{
		for (row = 0; row < level->rows; row++)
		{
			brick_draw(level->wall[line][row], renderer, media);
		}
	}
}