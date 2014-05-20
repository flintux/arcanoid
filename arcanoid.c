/****************************************************************************/
/*																			*/
/*kind of arkanoid game to play around with SDL								*/
/*																			*/
/****************************************************************************/

#include "arcanoid.h"

int main(int argc, char* args[])
{
	if (game_init() && game_media_load())
	{
		game_loop();
	}
	game_close();
	return 0;
}


