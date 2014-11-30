#include "game.h"

int main(int argc, char **argv)
{
	Game *game;
	game = new Game(WIDTH, HEIGHT);

	while (game->running){

		// process events
		game->process_events();

		game->move_sprite();
		game->render();
	}

	game->sound.destroy();
	game->video->quit();
}
