#include "media.h"



SDL_Texture* media_load_image(SDL_Renderer* gameRenderer, char *path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}


SDL_Texture* media_load_text(SDL_Renderer* gameRenderer, TTF_Font *gameFont, char *text)
{

	SDL_Texture *texture = NULL;
	SDL_Surface *temporarySurface = NULL;
	SDL_Color textColor = { 255, 255, 255 };
	temporarySurface = TTF_RenderText_Blended(gameFont, text, textColor);
	if (temporarySurface == NULL)
	{
		printf("Unable to create text: %s. Error: %s", text, TTF_GetError);
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(gameRenderer, temporarySurface);
		if (texture == NULL)
		{
			printf("Error creating texture from surface for text: %s", text);
		}
	}

	SDL_FreeSurface(temporarySurface);

	return texture;
}



/*
Loads all media (pictures, fonts and music need by the game
*/
GameMedia media_load(SDL_Renderer *gameRenderer)
{
	GameMedia media;
	//media = (GameMedia *) malloc(sizeof(GameMedia));

	media.background = media_load_image(gameRenderer, "media/background_640_480.png");
	if (media.background == NULL)
	{
		printf("Failed to load background image!\n");
		//return 0;
	}

	media.player = media_load_image(gameRenderer, "media/bar_80_20.png");
	if (media.player == NULL)
	{
		printf("Failed to load player image!\n");
		//return 0;
	}
	
	/*else
	{
		SDL_QueryTexture(media.player, NULL, NULL, &player.playerRect.w, &player.playerRect.h);
		player.playerRect.x = SCREEN_WIDTH / 2 - player.playerRect.w / 2;
		player.playerRect.y = SCREEN_HEIGHT - 40 - player.playerRect.h;
	}*/

	//Load ball and init position
	media.ball = media_load_image(gameRenderer, "media/ball_15.png");
	if (media.ball == NULL)
	{
		printf("Failed to load ball image!\n");
		//return 0;
	}
	/*else
	{
		SDL_QueryTexture(media.ball, NULL, NULL, &ball.ballRect.w, &ball.ballRect.h);
		ball.ballRect.x = SCREEN_WIDTH / 2 - ball.ballRect.w / 2;
		ball.PositionX = ball.ballRect.x;
		ball.ballRect.y = SCREEN_HEIGHT - 40 - player.playerRect.h - ball.ballRect.h;
		ball.PositionY = ball.ballRect.y;

	}*/

	media.brickNormal = media_load_image(gameRenderer, "media/brick_normal.png");
	if (media.brickNormal == NULL)
	{
		printf("Failed to load brick image!\n");
		//return 0;
	}
	/*else
	{
	SDL_QueryTexture(media.brickNormal, NULL, NULL, &brick.rect.w, &brick.rect.h);
	brick.rect.x = SCREEN_WIDTH / 2 - brick.rect.w / 2;
	brick.rect.y = SCREEN_HEIGHT - 400 - brick.rect.h;
	}*/

	media.brickDualOne = media_load_image(gameRenderer, "media/brick_dual_one.png");
	if (media.brickDualOne == NULL)
	{
		printf("Failed to load brickDualOne image!\n");
		//return 0;
	}
	media.brickDualTwo = media_load_image(gameRenderer, "media/brick_dual_two.png");
	if (media.brickDualTwo == NULL)
	{
		printf("Failed to load brickDualTwo image!\n");
		//return 0;
	}
	media.brickTripleOne = media_load_image(gameRenderer, "media/brick_triple_one.png");
	if (media.brickTripleOne == NULL)
	{
		printf("Failed to load brickTripleOne image!\n");
		//return 0;
	}
	media.brickTripleTwo = media_load_image(gameRenderer, "media/brick_triple_two.png");
	if (media.brickTripleTwo == NULL)
	{
		printf("Failed to load brickTripleTwo image!\n");
		//return 0;
	}
	media.brickTripleThree = media_load_image(gameRenderer, "media/brick_triple_three.png");
	if (media.brickTripleThree == NULL)
	{
		printf("Failed to load brickTripleThree image!\n");
		//return 0;
	}
	media.brickSuperman = media_load_image(gameRenderer, "media/brick_superman.png");
	if (media.brickSuperman == NULL)
	{
		printf("Failed to load brickSuperman image!\n");
		//return 0;
	}

	//load music
	/*
	media.music = Mix_LoadMUS("media/game_of_throne_ogg.ogg");
	if (media.music == NULL)
	{
		printf("Could not load music for game! %s", Mix_GetError());
		//return 0;
	}*/
	/*else
	{
		printf("Setting game volume: %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME / 2));
		printf("New volume set to : %d\n", Mix_VolumeMusic(-1));
		// Start music
		printf("starting music, press S to pause/resume\n");
		Mix_PlayMusic(media.music, -1);

	}*/

	//load font
	media.font = TTF_OpenFont("media/thickhea.ttf", 36);
	if (media.font == NULL)
	{
		printf("Could not load font! Error: %s\n", TTF_GetError());
	}
	else
	{
		media.textGameOver = media_load_text(gameRenderer, media.font, "Game Over, Loulou t'es null!!!");
	}

	media.music = Mix_LoadMUS("media/music.ogg");
	if (media.music == NULL)
	{
		printf("Could not load music! Error : %s\n", Mix_GetError());
	}
	return media;
}


/*
Destroys all media loaded my media_load
*/
void media_close(GameMedia *media)
{
	SDL_DestroyTexture(media->background);
	media->background = NULL;
	SDL_DestroyTexture(media->player);
	media->player = NULL;
	SDL_DestroyTexture(media->ball);
	media->ball = NULL;
	SDL_DestroyTexture(media->brickNormal);
	media->brickNormal = NULL;
	SDL_DestroyTexture(media->brickDualOne);
	media->brickDualOne = NULL;
	SDL_DestroyTexture(media->brickDualTwo);
	media->brickDualTwo = NULL;
	SDL_DestroyTexture(media->brickTripleOne);
	media->brickTripleOne = NULL;
	SDL_DestroyTexture(media->brickTripleTwo);
	media->brickTripleTwo = NULL;
	SDL_DestroyTexture(media->brickTripleThree);
	media->brickTripleThree = NULL;
	SDL_DestroyTexture(media->brickSuperman);
	media->brickSuperman = NULL;

	SDL_DestroyTexture(media->textGameOver);
	media->textGameOver = NULL;
	
	Mix_FreeMusic(media->music);
	media->music = NULL;

	TTF_CloseFont(media->font);

	//free(media);
}
