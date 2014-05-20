/**********************************************************************/
/*                                                                    */
/*main game functions for arkanoid game                               */
/*                                                                    */
/**********************************************************************/

#include "game.h"

GameSDLSetup *gameSetup = NULL;
GameMedia *media = NULL;
Game *game = NULL;

GameSDLSetup* game_setup_create(void)
{
	GameSDLSetup *gameSetup = NULL;

	gameSetup = malloc(sizeof *gameSetup);
	if (gameSetup == NULL)
	{
		printf("error creating game SDL setup!\n");
		return gameSetup;
	}
	gameSetup->window = NULL;
	gameSetup->renderer = NULL;
	gameSetup->height = SCREEN_HEIGHT;
	gameSetup->width = SCREEN_WIDTH;
	return gameSetup;
}

void game_setup_destroy(GameSDLSetup* gameSetup)
{
	if (gameSetup != NULL)
	{
		SDL_DestroyRenderer(gameSetup->renderer);
		gameSetup->renderer = NULL;
		SDL_DestroyWindow(gameSetup->window);
		gameSetup->window = NULL;
		free(gameSetup);
		gameSetup = NULL;
	}
}

Game* game_create(GameSDLSetup *gameSetup)
{
	Game *game = NULL;

	game = malloc(sizeof *game);
	if (game == NULL)
	{
		printf("error cannot allocate game memory\n");
		return game;
	}
	game->gameSetup = gameSetup;
	game->status = GAME_START;
	game->level = level_first(gameSetup->renderer, media);
	game->player = player_create(media);
	player_position(game->player, gameSetup->width / 2 - game->player->playerRect.w / 2, gameSetup->height - 40 - game->player->playerRect.h);
	game->ball = ball_create(media);
	ball_position(game->ball, gameSetup->width / 2 - game->ball->ballRect.w / 2, player_top(game->player) - game->ball->ballRect.h);
	return game;
}

void game_destroy(Game *game)
{
	if (game != NULL)
	{
		level_destroy(game->level);
		game->level = NULL;
		ball_destroy(game->ball);
		game->ball = NULL;
		player_destroy(game->player);
		game->player = NULL;
		free(game);
		game = NULL;
	}
}

int game_init(void)
{
	gameSetup = game_setup_create();
	if (gameSetup == NULL)
	{
		printf("erreur creating game setup\n");
		return 0;
	}
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}
	else
	{
		/* set texture filtering to linear */
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		/* create window */
		gameSetup->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gameSetup->width, gameSetup->height, SDL_WINDOW_SHOWN);
		if (gameSetup->window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return 0;
		}
		else
		{
			/* create a renderer for window */
			gameSetup->renderer = SDL_CreateRenderer(gameSetup->window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
			if (gameSetup->renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				return 0;
			}
			else
			{
				/* init the renderer color for clearing screen */
				SDL_SetRenderDrawColor(gameSetup->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
				/* init image loadig with PNG */
				int imgFlags = (IMG_INIT_PNG | IMG_INIT_JPG);
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return 0;
				}
				/* init sound */
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
					return 0;
				}
				/* init font rendering */
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not be loaded! Error: %s\n", TTF_GetError());
					return 0;
				}
				SDL_ShowCursor(0);
			}
		}
	}
	return 1;
}

/*
cleans up all memory used (textures, music...)
closes all inited SDL functions
*/
void game_close(void)
{
	game_destroy(game);
	media_close(media);
	game_setup_destroy(gameSetup);
	/* quit SDL subsystems */
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

/*
Loads all media (pictures, fonts and music need by the game)
*/
int game_media_load(void)
{
	media = media_load(gameSetup->renderer);
	if (media == NULL)
	{
		return 0;
	}
	if (media->player == NULL)
	{
		printf("Failed to load player image!\n");
		return 0;
	}
	if (media->background == NULL)
	{
		printf("Failed to load background image!\n");
		return 0;
	}
	if (media->ball == NULL)
	{
		printf("Failed to load ball image!\n");
		return 0;
	}
	if (media->music == NULL)
	{
		printf("Could not load music for game! %s", Mix_GetError());
		return 0;
	}
	else
	{	
		printf("Setting game volume: %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME / 2));
		printf("New volume set to : %d\n", Mix_VolumeMusic(-1));
		/* Start music */
		printf("starting music, press S to pause/resume\n");
		Mix_PlayMusic(media->music, -1);
	}
	if (media->font == NULL)
	{
		printf("Could not load font! Error: %s\n", TTF_GetError());
		return 0;
	}
	return 1;
}

void ball_update(Ball *ball)
{
	int line = 0;
	int row = 0;
	int collision = 0;

	if (!(ball->timer < SDL_GetTicks())) /* is it time to move ? */
	{
		return;
	}
	if ((ball_check_wall_collision(ball, gameSetup->width, gameSetup->height)) == COLLISION_SIDE_BOTTOM)
	{
		if (game->player->lifes > 0)
		{
			game->status = GAME_LOST;
			game->player->lifes--;
			return;
		}
		else
		{
			game->status = GAME_END;
			return;
		}
	}
	ball_check_player_collision(ball, game->player);
	if (ball->ballSpeedY > 0)
	{
		for (line = 0; line < game->level->lines && !(collision); line++)
		{
			if (ball->ballSpeedX > 0)
			{
				for (row = 0; row < game->level->rows && !(collision); row++)
				{
					if (ball_check_brick_collision(ball, game->level->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						game->level->destroyableBricks -= brick_collided(game->level->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
						break;
					}
				}
			}
			else
			{
				for (row = game->level->rows - 1; row >= 0 && !(collision); row--)
				{
					if (ball_check_brick_collision(ball, game->level->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						game->level->destroyableBricks -= brick_collided(game->level->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
					}
				}
			}
		}
	}
	else
	{
		for (line = game->level->lines - 1; line >= 0 && !(collision); line--)
		{
			if (ball->ballSpeedX > 0)
			{

				for (row = 0; row < game->level->rows && !(collision); row++)
				{
					if (ball_check_brick_collision(ball, game->level->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						game->level->destroyableBricks -= brick_collided(game->level->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
					}
				}
			}
			else
			{
				for (row = game->level->rows - 1; row >= 0 && !(collision); row--)
				{
					if (ball_check_brick_collision(ball, game->level->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						game->level->destroyableBricks -= brick_collided(game->level->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
					}
				}
			}
		}
	}
	ball_move(ball);
	ball->timer = SDL_GetTicks() + ball->updateSpeed; /* update for next test */
}


void game_reset(void)
{
	game_destroy(game);
	game = game_create(gameSetup);
}

void game_loop(void)
{
	game_reset();
	while (game->status != GAME_EXIT)
	{
		while (SDL_PollEvent(&gameSetup->event) != 0)
		{
			game_event();
		}
		if (game->status == GAME_START)
		{
			player_move(game->player, game->ball, 0, gameSetup->width);
		}
		if (game->status == GAME_RUNNING)
		{
			ball_update(game->ball);
			player_move(game->player, game->ball, 1, gameSetup->width);
		}
		if (game->level->destroyableBricks == 0)
		{
			game->status = GAME_FINISHED;
		}
		game_draw();
		if (game->status == GAME_LOST)
		{
			SDL_Delay(2000);
			game->status = GAME_START;
			player_position(game->player, gameSetup->width / 2 - game->player->playerRect.w / 2, gameSetup->height - 40 - game->player->playerRect.h);
			ball_destroy(game->ball);
			game->ball = ball_create(media);
			ball_position(game->ball, gameSetup->width / 2 - game->ball->ballRect.w / 2, player_top(game->player) - game->ball->ballRect.h);
		}
		if (game->status == GAME_FINISHED)
		{
			SDL_Delay(3000);
			game->level = level_next(game->level, gameSetup->renderer, media);
			game->status = GAME_START;
			player_position(game->player, gameSetup->width / 2 - game->player->playerRect.w / 2, gameSetup->height - 40 - game->player->playerRect.h);
			ball_destroy(game->ball);
			game->ball = ball_create(media);
			ball_position(game->ball, gameSetup->width / 2 - game->ball->ballRect.w / 2, player_top(game->player) - game->ball->ballRect.h);
		}
	}
}

void game_pause(Game *game)
{
	static GameStatus previousGameStatus = GAME_START;

	if (game->status != GAME_PAUSE)
	{
		previousGameStatus = game->status;
		game->status = GAME_PAUSE;
	}
	else
	{
		game->status = previousGameStatus;
	}
}

void game_event(void)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (gameSetup->event.type == SDL_QUIT)
	{
		game->status = GAME_EXIT;
	}
	else if (gameSetup->event.type == SDL_KEYDOWN)
	{

		if (currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
		{
			game->player->direction = DIRECTION_NONE;
		}
		else
		{
			if (currentKeyStates[SDL_SCANCODE_LEFT])
			{
				game->player->direction = DIRECTION_LEFT;
			}
			else if (currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				game->player->direction = DIRECTION_RIGHT;
			}
		}
		if (currentKeyStates[SDL_SCANCODE_SPACE] && gameSetup->event.key.repeat == 0)
		{
			if (game->status == GAME_START)
			{
				game->status = GAME_RUNNING;
			}
			else if (game->status == GAME_PAUSE)
			{
				game_pause(game);
			}
		}
		if (currentKeyStates[SDL_SCANCODE_P] && gameSetup->event.key.repeat == 0)
		{
			game_pause(game);
		}
		if (currentKeyStates[SDL_SCANCODE_Q] | currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			game->status = GAME_EXIT;
		}
		if (currentKeyStates[SDL_SCANCODE_R] && gameSetup->event.key.repeat == 0)
		{
			/*for (int test = 0; test < 100; test++)  used for testing memory leak*/
			if (game->status == GAME_END)
			{
				game_reset();
			}
		}
		if (currentKeyStates[SDL_SCANCODE_S] && gameSetup->event.key.repeat == 0)
		{
			if (Mix_PlayingMusic() == 0)
			{
				Mix_PlayMusic(media->music, -1);
			}
			else
			{
				if (Mix_PausedMusic() == 1)
				{
					Mix_ResumeMusic();
				}
				else
				{
					Mix_PauseMusic();
				}
			}
		}
		if (currentKeyStates[SDL_SCANCODE_L] && gameSetup->event.key.repeat == 0)
		{
			Level *newLevel = NULL;
			newLevel = level_next(game->level, gameSetup->renderer, media);
			if (newLevel != NULL)
			{
				level_destroy(game->level);
				game->level = newLevel;
			}
		}
	}
	else if (gameSetup->event.type == SDL_KEYUP)
	{
		game->player->direction = DIRECTION_NONE;
		if (currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
		{
			game->player->direction = DIRECTION_NONE;
		}
		else
		{
			if (currentKeyStates[SDL_SCANCODE_LEFT])
			{
				game->player->direction = DIRECTION_LEFT;
			}
			else if (currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				game->player->direction = DIRECTION_RIGHT;
			}
		}
	}
}

void game_draw(void)
{
	/* clear screen */
	SDL_RenderClear(gameSetup->renderer);
	/* render background */
	SDL_RenderCopy(gameSetup->renderer, media->background, NULL, NULL);
	/* render level */
	level_draw(game->level, gameSetup->renderer, media);
	/* draw remaining lifes */
	int i;
	SDL_Rect lifes;
	lifes = game->ball->ballRect;
	for (i = 0; i < game->player->lifes; i++)
	{
		lifes.x = 20 + i * 20;
		lifes.y = gameSetup->height - 30;
		SDL_RenderCopy(gameSetup->renderer, game->ball->ball, NULL, &lifes);
	}
	/* render ball */
	SDL_RenderCopy(gameSetup->renderer, game->ball->ball, NULL, &game->ball->ballRect);
	/* render player */
	SDL_RenderCopy(gameSetup->renderer, game->player->sprite, NULL, &game->player->playerRect);
	/* render game over */
	if (game->status == GAME_END)
	{
		SDL_Rect textRect;
		SDL_QueryTexture(media->textGameOver, NULL, NULL, &textRect.w, &textRect.h);
		textRect.x = (gameSetup->width - textRect.w) / 2;
		textRect.y = (gameSetup->height - textRect.h) / 2;
		SDL_RenderCopy(gameSetup->renderer, media->textGameOver, NULL, &textRect);
	}
	/* render level complete */
	else if (game->status == GAME_FINISHED)
	{
		SDL_Rect textRect;
		SDL_QueryTexture(media->textLevelComplete, NULL, NULL, &textRect.w, &textRect.h);
		textRect.x = (gameSetup->width - textRect.w) / 2;
		textRect.y = (gameSetup->height - textRect.h) / 2;
		SDL_RenderCopy(gameSetup->renderer, media->textLevelComplete, NULL, &textRect);
	}
	/* render life lost */
	else if (game->status == GAME_LOST)
	{
		SDL_Rect textRect;
		SDL_QueryTexture(media->textLiveLost, NULL, NULL, &textRect.w, &textRect.h);
		textRect.x = (gameSetup->width - textRect.w) / 2;
		textRect.y = (gameSetup->height - textRect.h) / 2;
		SDL_RenderCopy(gameSetup->renderer, media->textLiveLost, NULL, &textRect);
	}
	/* update screen */
	SDL_RenderPresent(gameSetup->renderer);
}