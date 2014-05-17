#include "level.h"




Level level_create_level(int number, int lines, int rows, GameMedia* media)
{
	//srand(time(NULL));
	srand(SDL_GetTicks());
	BrickKind kind;
	Level newLevel;
	int i, j;
	newLevel.number = number;
	if (rows > 20) rows = 20;
	if (lines > 24) lines = 24;
	newLevel.lines = lines;
	newLevel.rows = rows;
	newLevel.speedFactor = 1;

	for (i = 0; i < lines; i++)
	{
		for (j = 0; j < rows; j++)
		{
			switch (rand() % 5)
			{
			case 0: kind = BRICK_NONE; break;
			case 1: kind = BRICK_NORMAL; break;
			case 2: kind = BRICK_DUAL; break;
			case 3: kind = BRICK_TRIPLE; break;
			case 4: kind = BRICK_SUPERMAN; break;

			}
			newLevel.wall[i][j] = brick_create(kind, 12 + j*50, i*25, media);
		}
	}
	return newLevel;
}

void level_draw(Level *level, SDL_Renderer *renderer, GameMedia* media)
{
	int i, j;
	for (i = 0; i < level->lines; i++)
	{
		for (j = 0; j < level->rows; j++)
		{
			brick_draw(&level->wall[i][j], renderer, media);
		}
	}
}