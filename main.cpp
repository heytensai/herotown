#include "game.h"

int main(int argc, char **argv)
{
	Game *game;
	game = new Game(WIDTH, HEIGHT);

	while (1){
		game->intro_screen();

		if (game->exit_requested){
			break;
		}

		game->start();
		while (game->running){

			// process events
			game->process_inputs();
			game->process_events();

			game->create_random_coin();
			game->move_hero();
			game->render();
		}
		game->end();
	}

	game->sound.destroy();
	game->video->quit();

	delete game;
}
