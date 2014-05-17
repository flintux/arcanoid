#ifndef INC_GAME
#define INC_GAME

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


//functions prototypes
int game_init(void);
void game_close(void);

int game_media_load(void);

void game_loop(void);

void ball_update(Ball *ball);

void game_reset(void);

#endif

