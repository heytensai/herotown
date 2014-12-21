#include "bombycoinygame.h"
#include "jumpygame.h"

int main(int argc, char **argv)
{
	Video video(WIDTH, HEIGHT);
	video.init();
	video.create_window();
	video.load_background("resources/background.png");

	Sound sound;
	sound.init();

	Game *game;
	//game = new BombyCoinyGame(&video, &sound);
	game = new JumpyGame(&video, &sound);

	while (1){
		if (game->intro_screen()){
			break;
		}

		game->start();
		game->main_loop();
		game->score_screen();
		game->end();
	}
	delete game;

	sound.destroy();
	video.quit();
}
