#include "game.h"

Game *game;

int main(int argc, char **argv)
{
	game = new Game(WIDTH, HEIGHT);

	while (game->running){

		// process events
		game->process_events();

		game->draw_background();

		game->draw_sprite();

		// blit pixels
		game->video->blit();
	}

	game->sound.destroy();
	game->video->quit();
}
