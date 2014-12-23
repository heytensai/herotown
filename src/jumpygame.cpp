#include "jumpygame.h"

JumpyGame::JumpyGame(Video *video, Sound *sound)
	: Game(video, sound)
{
	running = true;
	run_once = false;
	gravity.direction = 3.14;
	gravity.speed = 2;
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

	// global keys
	if (state[SDL_SCANCODE_ESCAPE]){
		running = false;
	}
	if (state[SDL_SCANCODE_TAB]){
		hero[0]->location.x = 30;
		hero[0]->location.y = 300;
		hero[0]->velocity.speed = 0;
	}

	//reset heros
	hero[0]->motion.movement.x = 0;
	hero[1]->motion.movement.x = 0;

	int run_0 = 1;
	if (state[SDL_SCANCODE_LSHIFT]){
		run_0 = 2;
	}
	if (state[SDL_SCANCODE_SPACE]){
		//somehow need to know if hero is on the ground
		if (hero[0]->can_jump){
			hero[0]->can_jump = false;
			hero[0]->velocity.direction = 0;
			hero[0]->velocity.speed = 30;
		}
	}
	if (state[SDL_SCANCODE_A]){
		hero[0]->motion.movement.x = -Sprite::step * run_0;
	}
	if (state[SDL_SCANCODE_D]){
		hero[0]->motion.movement.x = Sprite::step * run_0;
	}

	int run_1 = 1;
	if (state[SDL_SCANCODE_KP_0]){
		run_1 = 2;
	}
	if (state[SDL_SCANCODE_KP_ENTER]){
		//somehow need to know if hero is on the ground
		if (hero[1]->can_jump){
			hero[1]->can_jump = false;
			hero[1]->velocity.direction = 0;
			hero[1]->velocity.speed = 30;
		}
	}
	if (state[SDL_SCANCODE_LEFT]){
		hero[1]->motion.movement.x = -Sprite::step * run_1;
	}
	if (state[SDL_SCANCODE_RIGHT]){
		hero[1]->motion.movement.x = Sprite::step * run_1;
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
	hero[1]->render();
	video->finish_render();
}

void JumpyGame::move_hero(int i)
{
	if (hero[i]->motion.movement.x < 0){
		hero[i]->set_animation(Animation::WALK_LEFT);
		hero[i]->location.x += hero[i]->motion.movement.x;
		if (hero[i]->location.x < 0){
			hero[i]->location.x = 0;
		}
	}
	else if (hero[i]->motion.movement.x > 0){
		hero[i]->set_animation(Animation::WALK_RIGHT);
		hero[i]->location.x += hero[i]->motion.movement.x;
		if (hero[i]->location.x > WIDTH){
			hero[i]->location.x = WIDTH;
		}
	}
	else{
		hero[i]->set_animation(Animation::NONE);
	}

	// apply velocity
	hero[i]->location.y -= hero[i]->velocity.speed;

	// adjust velocity for gravity
	hero[i]->velocity.speed -= gravity.speed;
	if (hero[i]->velocity.speed < Hero::TERMINAL_VELOCITY){
		hero[i]->velocity.speed = Hero::TERMINAL_VELOCITY;
	}

	// apply gravity
	hero[i]->location.y += gravity.speed;

	// hard coded floor
	if (hero[i]->location.y >= 300){
		hero[i]->can_jump = true;
		hero[i]->location.y = 300;
	}
}

void JumpyGame::init_hero()
{
	hero[0] = new Yoshi(video);
	hero[0]->location.x = 30;
	hero[0]->location.y = 300;

	hero[1] = new Mario(video);
	hero[1]->location.x = 500;
	hero[1]->location.y = 300;
}

void JumpyGame::main_loop()
{
	printf("JumpyGame::main_loop()\n");
	while (running){
		process_inputs();
		process_events();
		move_hero(0);
		move_hero(1);
		render();
	}
}

