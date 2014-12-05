#include "game.h"

int main(int argc, char **argv)
{
	Game *game;
	game = new Game(WIDTH, HEIGHT);

	while (game->running){

		// process events
		game->process_inputs();
		game->process_events();

		game->move_hero();
		game->render();
	}

	game->sound.destroy();
	game->video->quit();

	delete game;
}
