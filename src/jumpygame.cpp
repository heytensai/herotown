#include "jumpygame.h"

JumpyGame::JumpyGame(Video *video, Sound *sound)
	: Game(video, sound)
{
	running = true;
	run_once = false;
	gravity.direction = 3.14;
	gravity.speed = 3;
}

JumpyGame::~JumpyGame()
{
}

void JumpyGame::init_blocks()
{
	for (int i=0; i<BLOCKS; i++){
		blocks[i] = NULL;
	}
	for (int i=0; (i*32)<=video->width; i++){
		blocks[i] = new Block(video);
		blocks[i]->location.x = i * 32;
		blocks[i]->location.y = 340;
	}
}

void JumpyGame::start()
{
	printf("JumpyGame::start()\n");
	running = true;

	init_hero();
	init_blocks();
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
	int numkeys;
	const Uint8 *state = SDL_GetKeyboardState(&numkeys);

	if (state[SDL_SCANCODE_SPACE]){
		//somehow need to know if hero is on the ground
		if (hero[0]->can_jump){
			hero[0]->can_jump = false;
			hero[0]->velocity.direction = 0;
			hero[0]->velocity.speed = 20;
		}
	}
	if (state[SDL_SCANCODE_ESCAPE]){
		running = false;
	}
	if (state[SDL_SCANCODE_TAB]){
		hero[0]->location.x = 30;
		hero[0]->location.y = 300;
		hero[0]->velocity.speed = 0;
	}
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
	for (int i=0; i<BLOCKS; i++){
		if (blocks[i] != NULL){
			blocks[i]->render();
		}
	}
	hero[0]->render();
	video->finish_render();
}

void JumpyGame::move_hero()
{
	hero[0]->set_animation(Animation::WALK_RIGHT);
	hero[0]->location.x++;

	// apply velocity
	hero[0]->location.y -= hero[0]->velocity.speed;

	// adjust velocity for gravity
	hero[0]->velocity.speed -= gravity.speed;
	if (hero[0]->velocity.speed < 0){
		hero[0]->velocity.speed = 0;
	}

	// apply gravity
	hero[0]->location.y += gravity.speed;

	// hard coded floor
	if (hero[0]->location.y >= 300){
		hero[0]->can_jump = true;
		hero[0]->location.y = 300;
	}
}

void JumpyGame::init_hero()
{
	hero[0] = new Yoshi(video);
	hero[0]->location.x = 30;
	hero[0]->location.y = 300;
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

