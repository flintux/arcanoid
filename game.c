/**********************************************************************/
/*                                                                    */
/*main game functions for arkanoid game                               */
/*                                                                    */
/**********************************************************************/


#include "game.h"

GameSDLSetup *gameSetup = NULL;

GameMedia *media = NULL;

Game *game = NULL;

Ball *ball = NULL;

Player* player = NULL;;

Level* levelOne = NULL;



GameSDLSetup* game_setup_create(void)
{
	GameSDLSetup *gameSetup = NULL;
	gameSetup = (GameSDLSetup *) malloc(sizeof(GameSDLSetup));
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
	if (gameSetup == NULL)
	{
		return;
	}
	SDL_DestroyRenderer(gameSetup->renderer);
	gameSetup->renderer = NULL;
	SDL_DestroyWindow(gameSetup->window);
	gameSetup->window = NULL;
	free(gameSetup);
	gameSetup = NULL;
}


Game* game_create(GameSDLSetup *gameSetup)
{
	Game *game = NULL;
	game = (Game *)malloc(sizeof(Game));
	if (game == NULL)
	{
		printf("error cannot allocate game memory\n");
		return game;
	}
	game->status = GAME_START;

	return game;
}

void game_destroy(Game *game)
{
	if (game != NULL)
	{
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
		gameSetup->window = SDL_CreateWindow("Arcanoid style game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gameSetup->width, gameSetup->height, SDL_WINDOW_SHOWN);
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
				SDL_SetRenderDrawColor(gameSetup->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				/* init image loadig with PNG */
				int imgFlags = IMG_INIT_PNG;
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
				//SDL_ShowCursor(0);
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
	ball_destroy(ball);

	player_destroy(player);

	level_destroy(levelOne);

	game_destroy(game);

	media_close(media);

	//Destroy window

	game_setup_destroy(gameSetup);

	//Quit SDL subsystems
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

	/*
	if (!ballMove)
	{
		return;
	}
	*/
	if (!(ball->timer < SDL_GetTicks())) /* is it time to move ? */
	{
		return;
	}

	if ((ball_check_wall_collision(ball, SCREEN_WIDTH, SCREEN_HEIGHT)) == COLLISION_SIDE_BOTTOM){
		game->status = GAME_END;
	}

	ball_check_player_collision(ball, player);

	if (ball->ballSpeedY > 0)
	{
		for (line = 0; line < levelOne->lines && !(collision); line++)
		{
			if (ball->ballSpeedX > 0)
			{
				for (row = 0; row < levelOne->rows && !(collision); row++)
				{
					if (ball_check_brick_collision(ball, levelOne->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						brick_collided(levelOne->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
						break;
					}
				}
			}
			else
			{
				for (row = levelOne->rows - 1; row >= 0 && !(collision); row--)
				{
					if (ball_check_brick_collision(ball, levelOne->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						brick_collided(levelOne->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
					}
				}
			}
		}
	}
	else
	{
		for (line = levelOne->lines - 1; line >= 0 && !(collision); line--)
		{
			if (ball->ballSpeedX > 0)
			{

				for (row = 0; row < levelOne->rows && !(collision); row++)
				{
					if (ball_check_brick_collision(ball, levelOne->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						brick_collided(levelOne->wall[line][row]);
						collision = 1;
						printf("destruction brick X: %i, Y: %i\n", row, line);
					}
				}
			}
			else
			{
				for (row = levelOne->rows - 1; row >= 0 && !(collision); row--)
				{
					if (ball_check_brick_collision(ball, levelOne->wall[line][row]) != COLLISION_SIDE_NONE)
					{
						brick_collided(levelOne->wall[line][row]);
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

	level_destroy(levelOne);
	levelOne = level_create_random(1, 24, 20, media);

	player_destroy(player);
	player = player_create(media);
	player->playerRect.x = SCREEN_WIDTH / 2 - player->playerRect.w / 2;
	player->playerRect.y = SCREEN_HEIGHT - 40 - player->playerRect.h; // normally not needed since player never moves on Y

	ball_destroy(ball);
	ball = ball_create(media);
	ball->ballRect.x = SCREEN_WIDTH / 2 - ball->ballRect.w / 2;
	ball->PositionX = (float)ball->ballRect.x;
	ball->ballRect.y = SCREEN_HEIGHT - 40 - player->playerRect.h - ball->ballRect.h;
	ball->PositionY = (float)ball->ballRect.y;
}


void game_loop(void){

	game_reset();

	while (game->status != GAME_EXIT){

		while (SDL_PollEvent(&gameSetup->event) != 0){
			//SDL_PollEvent(&gameEvent);
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (gameSetup->event.type == SDL_QUIT)
			{
				game->status = GAME_EXIT;
			}
			else if (gameSetup->event.type == SDL_KEYDOWN)
			{

				if (currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
				{
					player->direction = DIRECTION_NONE; // if also RIGHT key is pressed don't move
				}
				else
				{
					if (currentKeyStates[SDL_SCANCODE_LEFT])
					{
						player->direction = DIRECTION_LEFT;
					}
					if (currentKeyStates[SDL_SCANCODE_RIGHT])
					{
						player->direction = DIRECTION_RIGHT;
					}
				}
				if (currentKeyStates[SDL_SCANCODE_SPACE])
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
				if (currentKeyStates[SDL_SCANCODE_P])
				{
					game_pause(game);
				}
				if (currentKeyStates[SDL_SCANCODE_Q] | currentKeyStates[SDL_SCANCODE_ESCAPE])
				{
					game->status = GAME_EXIT;
				}
				if (currentKeyStates[SDL_SCANCODE_R])
				{
					if (game->status == GAME_END)
					{
						game_reset();
					}
				}
				if (currentKeyStates[SDL_SCANCODE_S])
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
			}
			else if (gameSetup->event.type == SDL_KEYUP)
			{
				player->direction = DIRECTION_NONE;
				if (currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
				{
					player->direction = DIRECTION_NONE; // if also RIGHT key is pressed don't move
				}
				else
				{
					if (currentKeyStates[SDL_SCANCODE_LEFT])
					{
						player->direction = DIRECTION_LEFT;
					}
					if (currentKeyStates[SDL_SCANCODE_RIGHT])
					{
						player->direction = DIRECTION_RIGHT;
					}
				}
			}
		}

		if (game->status == GAME_START)
		{
			player_move(player, ball, 0, SCREEN_WIDTH);
		}
		if (game->status == GAME_RUNNING)
		{
			ball_update(ball);
			player_move(player, ball, 1, SCREEN_WIDTH);
		}

		//Clear screen
		SDL_RenderClear(gameSetup->renderer);

		//Render background
		SDL_RenderCopy(gameSetup->renderer, media->background, NULL, NULL);

		//Render level
		level_draw(levelOne, gameSetup->renderer, media);

		//Render ball
		SDL_RenderCopy(gameSetup->renderer, ball->ball, NULL, &ball->ballRect);

		//Render player
		SDL_RenderCopy(gameSetup->renderer, player->sprite, NULL, &player->playerRect);

		//render game over
		if (game->status == GAME_END)
		{
			SDL_Rect textRect;
			SDL_QueryTexture(media->textGameOver, NULL, NULL, &textRect.w, &textRect.h);
			textRect.x = (SCREEN_WIDTH - textRect.w) / 2;
			textRect.y = 200;
			SDL_RenderCopy(gameSetup->renderer, media->textGameOver, NULL, &textRect);
		}

		//Update screen
		SDL_RenderPresent(gameSetup->renderer);

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