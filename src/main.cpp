#include "explorygame.h"

int main(int argc, char **argv)
{
	Video video(WIDTH, HEIGHT);
	video.init();
	video.create_window();
	video.load_background("resources/background.png");

	Sound sound;
	sound.init();

	Game *game = new ExploryGame(&video, &sound);

	if (game == NULL){
		printf("invalid game\n");
		return 1;
	}

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
