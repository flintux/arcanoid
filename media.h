#ifndef __MEDIA_H__
#define __MEDIA_H__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define MEDIA_BACKGROUND "media/background_640_480.png"
#define MEDIA_PLAYER "media/bar_80_20.png"
#define MEDIA_BALL "media/ball_15.png"
#define MEDIA_BRICK_NORMAL "media/brick_normal.png"
#define MEDIA_BRICK_DUAL_ONE "media/brick_dual_one.png"
#define MEDIA_BRICK_DUAL_TWO "media/brick_dual_two.png"
#define MEDIA_BRICK_TRIPLE_ONE "media/brick_triple_one.png"
#define MEDIA_BRICK_TRIPLE_TWO "media/brick_triple_two.png"
#define MEDIA_BRICK_TRIPLE_THREE "media/brick_triple_three.png"
#define MEDIA_BRICK_SUPERMAN "media/brick_superman.png"
#define MEDIA_FONT "media/thickhea.ttf"
#define MEDIA_FONT_SIZE 36
#define MEDIA_GAME_OVER "Game Over, Loulou t'es null!!!"
#define MEDIA_MUSIC "media/music.ogg"
#define MEDIA_FONT_WHITE_COLOR {255, 255, 255}

typedef struct GameMedia{
	SDL_Texture *background;
	SDL_Texture *player;
	SDL_Texture *ball;
	SDL_Texture *brickNormal;
	SDL_Texture *brickSuperman;
	SDL_Texture *brickDualOne;
	SDL_Texture *brickDualTwo;
	SDL_Texture *brickTripleOne;
	SDL_Texture *brickTripleTwo;
	SDL_Texture *brickTripleThree;
	SDL_Texture *textGameOver;
	Mix_Music *music;
	TTF_Font *font;
}GameMedia;

/*
loads an image to a texture
*/
SDL_Texture* media_load_image(SDL_Renderer* gameRenderer, char *path);

/*
creates a texture based on input text and font
*/
SDL_Texture* media_load_text(SDL_Renderer* gameRenderer, TTF_Font *gameFont, char *text);

/*
loads all media (pictures, fonts and music need by the game)
*/
GameMedia* media_load(SDL_Renderer *gameRenderer);

/*
destroys all media loaded my media_load
*/
void media_close(GameMedia *media);

#endif

