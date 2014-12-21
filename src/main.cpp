#include "bombycoinygame.h"

int main(int argc, char **argv)
{
	Video video(WIDTH, HEIGHT);
	video.init();
	video.create_window();
	video.load_background("resources/background.png");

	Sound sound;
	sound.init();

	BombyCoinyGame *game;
	game = new BombyCoinyGame(&video, &sound);

	while (1){
		if (game->intro_screen()){
			break;
		}

		game->start();
		game->main_loop();
		game->score_screen();
		game->end();
	}

	sound.destroy();
	video.quit();

	delete game;
}
