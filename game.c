/****************************************************************************/
/*																			*/
/*main game functions for arkanoid game										*/
/*																			*/
/****************************************************************************/


#include "game.h"

//Screen dimension constants
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

//global vars used for game

//The window we'll be rendering to
SDL_Window* gameWindow = NULL;

//Renderer
SDL_Renderer* gameRenderer = NULL;

//Event for event handling
SDL_Event gameEvent;

GameMedia media;

Ball ball;

Player player;

Brick brick;

Level levelOne;

//flag for game quit
int gameQuit = 0;

//flag for ball move
int ballMove = 0;

int gameStatus = 1;

int gamePause = 0;


int game_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gameWindow = SDL_CreateWindow("Arcanoid style game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN /*| SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_FULLSCREEN*/);
		if (gameWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return 0;
		}
		else
		{
			//Create renderer for window
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
			if (gameRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				return 0;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return 0;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
					return 0;
				}
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not be loaded! Error: %s\n", TTF_GetError());
					return 0;
				}

				// disable cursor
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
	//Destroy media
	
	//reset all pointers just in case
	player.player = NULL;
	ball.ball = NULL;

	media_close(&media);

	//Destroy window
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameRenderer = NULL;
	gameWindow = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


/*
Loads all media (pictures, fonts and music need by the game
*/
int game_media_load(void)
{
	//Loads the media required by the game

	media = media_load(gameRenderer);

	//Load player bar and init position
	player.player = media.player;
	if (player.player == NULL)
	{
		printf("Failed to load player image!\n");
		return 0;
	}
	else
	{
		SDL_QueryTexture(player.player, NULL, NULL, &player.playerRect.w, &player.playerRect.h);
		player.playerRect.x = SCREEN_WIDTH/2 - player.playerRect.w/2;
		player.playerRect.y = SCREEN_HEIGHT-40 - player.playerRect.h;
	}

	//background = media.background;
	if (media.background == NULL)
	{
		printf("Failed to load background image!\n");
		return 0;
	}

	//Load ball and init position
	ball.ball = media.ball;
	if (ball.ball == NULL)
	{
		printf("Failed to load ball image!\n");
		return 0;
	}
	else
	{
		SDL_QueryTexture(ball.ball, NULL, NULL, &ball.ballRect.w, &ball.ballRect.h);
		ball.ballRect.x = SCREEN_WIDTH / 2 - ball.ballRect.w / 2;
		ball.PositionX = ball.ballRect.x;
		ball.ballRect.y = SCREEN_HEIGHT - 40 - player.playerRect.h - ball.ballRect.h;
		ball.PositionY = ball.ballRect.y;

	}

	//load music
	if (media.music == NULL)
	{
		printf("Could not load music for game! %s", Mix_GetError());
		return 0;
	}
	else
	{	
		printf("Setting game volume: %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME / 2));
		printf("New volume set to : %d\n", Mix_VolumeMusic(-1));
		// Start music
		printf("starting music, press S to pause/resume\n");
		Mix_PlayMusic(media.music, -1);
		
	}

	//load font
	if (media.font == NULL)
	{
		printf("Could not load font! Error: %s\n", TTF_GetError());
		return 0;
	}

	//Load player bar and init position
	media.brickNormal;
	if (media.brickNormal == NULL)
	{
		printf("Failed to load brick image!\n");
		return 0;
	}
	else
	{
		SDL_QueryTexture(media.brickNormal, NULL, NULL, &brick.rect.w, &brick.rect.h);
		brick.rect.x = SCREEN_WIDTH / 2 - brick.rect.w / 2;
		brick.rect.y = SCREEN_HEIGHT - 400 - brick.rect.h;
	}
	return 1;
}



/*
Updates position of ball and checks eventual collisions.
*/
void ball_update(Ball *ball)
{
	int i = 0;
	int j = 0;
	int collision = 0;

	if (ballMove)	// if ball is in movement
	{
		//printf("ball ticks %li", SDL_GetTicks());
		if (ball->timer + ball->updateSpeed < SDL_GetTicks()) // if delay for moving ball passed
		{

			switch (ball_check_wall_collision(ball, SCREEN_WIDTH, SCREEN_HEIGHT)){
			case COLLISION_SIDE_BOTTOM:
				ballMove = 0;
				gameStatus = 0;
				puts("collision with bottom");
				break;
			case COLLISION_SIDE_LEFT:
				puts("collision with left");
				break;
			case COLLISION_SIDE_RIGHT:
				puts("collision with right");
				break;
			case COLLISION_SIDE_TOP:
				puts("collision with top");
				break;
			case COLLISION_SIDE_NONE: default:
				break;

			}

			ball_check_player_collision(ball, &player);

			if (ball->ballSpeedY > 0)
			{

				for (i = 0; i < levelOne.lines && !(collision); i++)
				{
					if (ball->ballSpeedX > 0)
					{

						for (j = 0; j < levelOne.rows && !(collision); j++)
						{
							if (levelOne.wall[i][j].kind != BRICK_NONE)
							{
								if (ball_check_brick_collision(ball, &levelOne.wall[i][j]) != COLLISION_SIDE_NONE)
								{
									brick_collided(&levelOne.wall[i][j]);
									//levelOne.wall[i][j].kind = BRICK_NONE;
									collision = 1;
									printf("destruction brick X: %i, Y: %i\n", j, i);
									break;
								}

							}
						}
					}
					else
					{
						for (j = levelOne.rows - 1; j >= 0 && !(collision); j--)
						{
							if (levelOne.wall[i][j].kind != BRICK_NONE)
							{
								if (ball_check_brick_collision(ball, &levelOne.wall[i][j]) != COLLISION_SIDE_NONE)
								{
									brick_collided(&levelOne.wall[i][j]);
									//levelOne.wall[i][j].kind = BRICK_NONE;
									collision = 1;
									printf("destruction brick X: %i, Y: %i\n", j, i);
								}
							}
						}
					}
				}
			}
			else
			{
				for (i = levelOne.lines - 1; i >= 0 && !(collision); i--)
				{
					if (ball->ballSpeedX > 0)
					{

						for (j = 0; j < levelOne.rows && !(collision); j++)
						{
							if (levelOne.wall[i][j].kind != BRICK_NONE)
							{
								if (ball_check_brick_collision(ball, &levelOne.wall[i][j]) != COLLISION_SIDE_NONE)
								{
									brick_collided(&levelOne.wall[i][j]);
									//levelOne.wall[i][j].kind = BRICK_NONE;
									collision = 1;
									printf("destruction brick X: %i, Y: %i\n", j, i);
								}

							}
						}
					}
					else
					{
						for (j = levelOne.rows - 1; j >= 0 && !(collision); j--)
						{
							if (levelOne.wall[i][j].kind != BRICK_NONE)
							{
								if (ball_check_brick_collision(ball, &levelOne.wall[i][j]) != COLLISION_SIDE_NONE)
								{
									brick_collided(&levelOne.wall[i][j]);
									//levelOne.wall[i][j].kind = BRICK_NONE;
									collision = 1;
									printf("destruction brick X: %i, Y: %i\n", j, i);
								}
							}
						}
					}
				}
			}
				//ball_check_brick_collision(ball, &brick);

			//move the ball
			//it would be better to take time * speed in order to avoid slow down if computer
			// doesn't process fast enough (in this case it should be fine :-)
			ball->PositionX += ball->ballSpeedX;
			ball->PositionY += ball->ballSpeedY;
			ball->ballRect.x = ball->PositionX;
			ball->ballRect.y = ball->PositionY;
			ball->timer = SDL_GetTicks();
		}
	}

}


void game_reset(void)
{
	ball.timer = SDL_GetTicks();
	player.keyTimer = SDL_GetTicks();
	ball.actualSpeed = -0.4;
	ball.ballSpeedX = ball.actualSpeed;
	ball.ballSpeedY = -ball.actualSpeed;
	player.direction = 0;
	player.speed = 2;
	player.playerRect.x = SCREEN_WIDTH / 2 - player.playerRect.w / 2;
	player.playerRect.y = SCREEN_HEIGHT - 40 - player.playerRect.h; // normally not needed since player never moves on Y
	ball.ballRect.x = SCREEN_WIDTH / 2 - ball.ballRect.w / 2;
	ball.PositionX = ball.ballRect.x;
	ball.ballRect.y = SCREEN_HEIGHT - 40 - player.playerRect.h - ball.ballRect.h;
	ball.PositionY = ball.ballRect.y;
	ballMove = 0;
	gamePause = 0;
	gameStatus = 1;

	levelOne = level_create_level(1, 24, 20, &media);

}


void game_loop(void){

	game_reset();

	while (!gameQuit){

		while (SDL_PollEvent(&gameEvent) != 0){
			//SDL_PollEvent(&gameEvent);
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (gameEvent.type == SDL_QUIT)
			{
				gameQuit = 1;
			}
			else if (gameEvent.type == SDL_KEYDOWN)
			{

				if (currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
				{
					player.direction = 0; // if also RIGHT key is pressed don't move
				}
				else
				{
					if (currentKeyStates[SDL_SCANCODE_LEFT])
					{
						player.direction = 2;
					}
					if (currentKeyStates[SDL_SCANCODE_RIGHT])
					{
						player.direction = 1;
					}
				}
				if (currentKeyStates[SDL_SCANCODE_SPACE])
				{
					ballMove = 1;
					gamePause = 0;
				}
				if (currentKeyStates[SDL_SCANCODE_P])
				{
					ballMove = 0;
					if (gamePause)
					{
						gamePause = 0;
						ballMove = 1;
					}
					else
					{
						gamePause = 1;
						ballMove = 0;
					}
				}
				if (currentKeyStates[SDL_SCANCODE_Q] | currentKeyStates[SDL_SCANCODE_ESCAPE])
				{
					gameQuit = 1;
				}
				if (currentKeyStates[SDL_SCANCODE_R])
				{
					if (!gameStatus)
					{
						game_reset();
					}
				}
				if (currentKeyStates[SDL_SCANCODE_S])
				{
					if (Mix_PlayingMusic() == 0)
					{
						Mix_PlayMusic(media.music, -1);
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
			else if (gameEvent.type == SDL_KEYUP)
			{
				player.direction = 0;
				if (currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
				{
					player.direction = 0; // if also RIGHT key is pressed don't move
				}
				else
				{
					if (currentKeyStates[SDL_SCANCODE_LEFT])
					{
						player.direction = 2;
					}
					if (currentKeyStates[SDL_SCANCODE_RIGHT])
					{
						player.direction = 1;
					}
				}

			}

		}

		if (gameStatus && !gamePause)
		{
			ball_update(&ball);

			movePlayer(&player, &ball, ballMove, SCREEN_WIDTH);
		}

		//Clear screen
		SDL_RenderClear(gameRenderer);

		//Render background
		SDL_RenderCopy(gameRenderer, media.background, NULL, NULL);


		//Render brick
		//SDL_RenderCopy(gameRenderer, brick.brick, NULL, &brick.rect);

		//Render level
		level_draw(&levelOne, gameRenderer, &media);

		//Render ball
		SDL_RenderCopy(gameRenderer, ball.ball, NULL, &ball.ballRect);

		//Render player
		SDL_RenderCopy(gameRenderer, player.player, NULL, &player.playerRect);

		//render game over
		if (!gameStatus)
		{
			SDL_Rect textRect;
			SDL_QueryTexture(media.textGameOver, NULL, NULL, &textRect.w, &textRect.h);
			textRect.x = (SCREEN_WIDTH - textRect.w) / 2;
			textRect.y = 200;
			SDL_RenderCopy(gameRenderer, media.textGameOver, NULL, &textRect);
		}

		//Update screen
		SDL_RenderPresent(gameRenderer);

	}
}