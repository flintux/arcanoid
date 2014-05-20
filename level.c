#include "level.h"




Level* level_create_random(int number, int lines, int rows, GameMedia* media)
{
	//srand(time(NULL));
	srand(SDL_GetTicks());
	BrickKind kind;
	Level *level = NULL;
	level = malloc(sizeof *level);
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


Level* level_load_file(char *path, SDL_Renderer *renderer, GameMedia* media)
{
	Level *level = NULL;
	FILE *file = NULL;
	int totalChars = 0;
	int line = 0;
	int row = 0;
	BrickKind kind = BRICK_NONE;

	int readChar;

	file = fopen(path, "r");
	if (!file)
	{
		printf("error loading file: %s\n", path);
		return level;
	}

	level = malloc(sizeof *level);
	if (!level)
	{
		printf("error allocating memory for level for %s", path);
		fclose(file);
		file = NULL;
		return level;
	}

	level->number = 0;
	level->speedFactor = LEVEL_SPEED_FACTOR;

	while ((readChar = getc(file)) != EOF)
	{
		switch (readChar)
		{
		case '\n':
			/* check if all rows are filled */
			if (line >= LEVEL_LINE_MAX)
			{
				printf("too many lines in file, skipping\n");
				break;
			}
			for (row; row < LEVEL_ROW_MAX;row++)
			{
				kind = BRICK_NONE;
				level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
			}
			line++;
			row = 0;
			break;
		case '0':
			if (row >= LEVEL_ROW_MAX || line >= LEVEL_LINE_MAX)
			{
				printf("too many lines or rows in file, skipping\n");
				break;
			}
			kind = BRICK_NONE;
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
			row++;
			break;
		case '1':
			if (row >= LEVEL_ROW_MAX || line >= LEVEL_LINE_MAX)
			{
				printf("too many lines or rows in file, skipping\n");
				break;
			}
			kind = BRICK_NORMAL;
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
			row++;
			break;
		case '2':
			if (row >= LEVEL_ROW_MAX || line >= LEVEL_LINE_MAX)
			{
				printf("too many lines or rows in file, skipping\n");
				break;
			}
			kind = BRICK_DUAL;
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
			row++;
			break;
		case '3':
			if (row >= LEVEL_ROW_MAX || line >= LEVEL_LINE_MAX)
			{
				printf("too many lines or rows in file, skipping\n");
				break;
			}
			kind = BRICK_TRIPLE;
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
			row++;
			break;
		case '9':
			if (row >= LEVEL_ROW_MAX || line >= LEVEL_LINE_MAX)
			{
				printf("too many lines or rows in file, skipping\n");
				break;
			}
			kind = BRICK_SUPERMAN;
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
			row++;
			break;
		default:
			printf("error, unrecognized brick kind, skipping\n");
			break;
		}
	}

	/* check if all lines are filled */
	for (line; line < LEVEL_LINE_MAX; line++)
	{
		for (row = 0; row < LEVEL_ROW_MAX; row++)
		{
			kind = BRICK_NONE;
			level->wall[line][row] = brick_create(kind, 12 + row * BRICK_WIDTH, line * BRICK_HEIGHT, media);
		}
	}
	level->rows = LEVEL_ROW_MAX;
	level->lines = LEVEL_LINE_MAX;

	fclose(file);

	return level;
}