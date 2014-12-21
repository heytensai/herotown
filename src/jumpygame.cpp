#include "jumpygame.h"

JumpyGame::JumpyGame(Video *video, Sound *sound)
	: Game(video, sound)
{
	running = true;
	run_once = false;
}

JumpyGame::~JumpyGame()
{
}

void JumpyGame::start()
{
	printf("JumpyGame::start()\n");
	running = true;

	init_hero();
}

void JumpyGame::end()
{
	printf("JumpyGame::end()\n");
	running = false;
}

bool JumpyGame::intro_screen()
{
	printf("JumpyGame::intro_screen()\n");
	return !running;
}

void JumpyGame::score_screen()
{
	printf("JumpyGame::score_screen()\n");
}

void JumpyGame::process_inputs()
{
}

void JumpyGame::process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				running = false;
			} break;
			case SDL_TEXTINPUT:
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				running = false;
				//ignore
			} break;
			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
			{
				//TODO: handle joystick plug/unplug events
				printf("joystick added or removed\n");
			} break;
			case SDL_WINDOWEVENT:
			{
			} break;
			default:
			{
				fprintf(stdout, "sdl event: %x\n", event.type);
			} break;
		}
	}

}

void JumpyGame::render()
{
	video->start_render();
	video->blit_background();
	hero[0]->render();
	video->finish_render();
}

void JumpyGame::move_hero()
{
	hero[0]->set_animation(Animation::WALK_RIGHT);
	hero[0]->motion.movement.y = 5;
	hero[0]->location.x++;
	hero[0]->location.y++;
}

void JumpyGame::init_hero()
{
	hero[0] = new Yoshi(video);
	hero[0]->location.x = 0;
	hero[0]->location.y = 0;
}

void JumpyGame::main_loop()
{
	printf("JumpyGame::main_loop()\n");
	while (running){
		process_inputs();
		process_events();
		move_hero();
		render();
	}
}

