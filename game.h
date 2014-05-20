#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "ball.h"
#include "player.h"
#include "media.h"
#include "brick.h"
#include "level.h"

/* window dimensions */
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

typedef enum GameStatus{
	GAME_START,
	GAME_PAUSE,
	GAME_RUNNING,
	GAME_FINISHED,
	GAME_END,
	GAME_EXIT
}GameStatus;

typedef struct GameSDLSetup{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	int width;
	int height;
}GameSDLSetup;

typedef struct Game{
	GameSDLSetup *gameSetup;
	Level *level;
	Player *player;
	Ball *ball;
	GameStatus status;
}Game;

/*
creates a GameSDLSetup
*/
GameSDLSetup* game_setup_create(void);

/*
destroys a GameSDLSetup
*/
void game_setup_destroy(GameSDLSetup* gameSetup);

/*
creates a game
*/
Game* game_create(GameSDLSetup *gameSetup);

/*
destroys a game
*/
void game_destroy(Game *game);

/*
inits game environement
*/
int game_init(void);

/*
cleans up all media, and quit subsystems
*/
void game_close(void);

/*
loads all media required for game to run
*/
int game_media_load(void);

/*
main game loop, runs until user exits
*/
void game_loop(void);

/*
updates ball position and check collisions
should be in ball "class"
*/
void ball_update(Ball *ball);

/*
resets game to default values for a restart
*/
void game_reset(void);

/*
pauses game
*/
void game_pause(Game *game);

/*
checks events from user interface
*/
void game_event(void);

/*
draws game elements into window
*/
void game_draw(void);

#endif

