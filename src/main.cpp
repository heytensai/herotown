#include "bombycoinygame.h"
#include "jumpygame.h"
#include "explorygame.h"

int main(int argc, char **argv)
{
	if (argc == 1){
		printf("no game requested\n");
		return 1;
	}

	Video video(WIDTH, HEIGHT);
	video.init();
	video.create_window();
	video.load_background("resources/background.png");

	Sound sound;
	sound.init();

	Game *game;

	if (strcmp("bombycoiny", argv[1]) == 0){
		game = new BombyCoinyGame(&video, &sound);
	}
	else if (strcmp("jumpy", argv[1]) == 0){
		game = new JumpyGame(&video, &sound);
	}
	else if (strcmp("explory", argv[1]) == 0){
		game = new ExploryGame(&video, &sound);
	}

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
