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
		while (game->running){

			// process events
			game->process_inputs();
			game->process_events();

			game->create_random_coin();
			game->move_heros();
			game->render();
		}
		game->end();
	}

	game->sound.destroy();
	game->video->quit();

	delete game;
}
