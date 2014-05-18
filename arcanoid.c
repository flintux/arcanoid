/****************************************************************************/
/*																			*/
/*kind of arkanoid game to play around with SDL								*/
/*																			*/
/****************************************************************************/

#include "arcanoid.h"


int main(int argc, char* args[])
{


	//Initialize everything and launch game if all ok
	if (game_init() && game_media_load())
	{
		game_loop();
	}

	//close the game (free ressources and all the rest)
	game_close();

	return 0;
}


