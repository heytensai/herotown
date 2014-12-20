#include "game.h"

int main(int argc, char **argv)
{
	Game *game;
	game = new Game(WIDTH, HEIGHT);

	while (1){
		if (game->intro_screen()){
			break;
		}

		game->start();
		game->main_loop();
		game->score_screen();
		game->end();
	}

	game->sound.destroy();
	game->video->quit();

	delete game;
}
