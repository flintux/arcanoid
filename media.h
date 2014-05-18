#ifndef _INC_MEDIA
#define _INC_MEDIA

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

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

