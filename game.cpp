#include "game.h"

Game::Game(int width, int height)
{
	this->width = width;
	this->height = height;
	running = 1;
	input_state = 0;

	video = new Video(width, height);
	video->init();
	video->create_window();
	video->load_background("resources/background.png");

	background.active = 0;
	background.movement.x = 0;
	background.movement.y = 0;

	init_hero();
	init_blocks();
	init_coins();

	sound.init();
	init_controller();

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
}

Game::~Game()
{
	if (hero[0] != NULL){
		delete hero[0];
		hero[0] = NULL;
	}
	for (int i=0; i<BLOCKS; i++){
		if (blocks[i] != NULL){
			delete blocks[i];
			blocks[i] = NULL;
		}
	}
	for (int i=0; i<COINS; i++){
		if (coins[i] != NULL){
			delete coins[i];
			coins[i] = NULL;
		}
	}
	if (video != NULL){
		delete video;
		video = NULL;
	}
	/*
	 * TODO this causes a double free in SDL
	for (int i=0; i<MAX_JOYDEV; i++){
		if (joy[i] != NULL){
			printf("closing joystick %i\n");
			SDL_JoystickClose(joy[i]);
			joy[i] = NULL;
		}
	}
	*/
}

void Game::add_coin(int x, int y, bool ignore_tick = false)
{
	if (!ignore_tick){
		Uint32 cur_tick = SDL_GetTicks();
		if (cur_tick - last_coin_added < COIN_TICKS){
			return;
		}
		last_coin_added = cur_tick;
	}
	for (int i=0; i<COINS; i++){
		if (coins[i] == NULL){
			coins[i] = new Sprite(20, 25);
			coins[i]->motion.active = 0;
			coins[i]->location.x = x;
			coins[i]->location.y = y;
			coins[i]->load_image(video->renderer, "resources/coin0.png");
			return;
		}
	}
}

void Game::init_coins()
{
	for (int i=0; i<COINS; i++){
		coins[i] = NULL;
	}
	for (int i=0; i<1; i++){
		add_coin(i * 30 + 200, 530, true);
	}
}

void Game::init_controller()
{
	for (int i=0; i<MAX_JOYDEV; i++){
		joy[i] = NULL;
	}

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	fprintf(stdout, "joy=%d\n", SDL_NumJoysticks());
	if (SDL_NumJoysticks() > 0){
		use_joy = true;
		for (int i=0; i<SDL_NumJoysticks(); i++){
			joy[i] = SDL_JoystickOpen(i);
			printf("Name: %s\n", SDL_JoystickNameForIndex(i));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy[i]));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy[i]));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy[i]));
			if (SDL_IsGameController(i)){
				printf("Is a game controller\n");
			}
			else{
				printf("Is NOT a game controller\n");
			}
		}
	}
	else{
		use_joy = true;
	}
}

/*
 * this must happen after video is created
 */
void Game::init_hero()
{
	hero[0] = new Hero(48, 64);
	hero[0]->location.x = 80;
	hero[0]->location.y = 520;

	hero[0]->motion.active = 0;
	hero[0]->motion.movement.x = 0;
	hero[0]->motion.movement.y = 0;

	hero[0]->enable_animation(8);
	hero[0]->load_animation(video->renderer, "resources/yoshi0.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi1.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi2.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi3.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi4.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi5.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi6.png");
	hero[0]->load_animation(video->renderer, "resources/yoshi7.png");

	hero[0]->load_image(video->renderer, "resources/yoshi0.png");

	hero[1] = new Hero(48, 64);
	hero[1]->location.x = 720;
	hero[1]->location.y = 520;

	hero[1]->motion.active = 0;
	hero[1]->motion.movement.x = 0;
	hero[1]->motion.movement.y = 0;

	hero[1]->enable_animation(8);
	hero[1]->load_animation(video->renderer, "resources/mario0.png");
	hero[1]->load_animation(video->renderer, "resources/mario1.png");
	hero[1]->load_animation(video->renderer, "resources/mario2.png");
	hero[1]->load_animation(video->renderer, "resources/mario3.png");
	hero[1]->load_animation(video->renderer, "resources/mario4.png");
	hero[1]->load_animation(video->renderer, "resources/mario5.png");
	hero[1]->load_animation(video->renderer, "resources/mario6.png");
	hero[1]->load_animation(video->renderer, "resources/mario7.png");

	hero[1]->load_image(video->renderer, "resources/mario0.png");
}

void Game::init_blocks()
{
	for (int i=0; i<BLOCKS; i++){
		blocks[i] = NULL;
	}
	for (int i=0; (i*32)<=width; i++){
		blocks[i] = new Sprite(32, 16);
		blocks[i]->motion.active = 0;
		blocks[i]->location.x = i * 32;
		blocks[i]->location.y = 550;
		blocks[i]->load_image(video->renderer, "resources/block.png");
	}
}

void Game::render()
{
	video->start_render();
	video->blit_background();
	for (int i=0; i<BLOCKS; i++){
		if (blocks[i] != NULL){
			blocks[i]->render(video->renderer);
		}
	}
	for (int i=0; i<COINS; i++){
		if (coins[i] != NULL){
			coins[i]->render(video->renderer);
		}
	}
	hero[0]->render(video->renderer);
	hero[1]->render(video->renderer);
	video->finish_render();
}

void Game::move_hero()
{
	// move hero[0] (if hero[0] is in bounds)
	if (hero[0]->motion.active){
		if (hero[0]->can_move('x')){
			hero[0]->location.x += hero[0]->motion.movement.x;
		}

		if (hero[0]->can_move('y')){
			hero[0]->location.y += hero[0]->motion.movement.y;
		}
	}

	// move hero[1] (if hero[1] is in bounds)
	if (hero[1]->motion.active){
		if (hero[1]->can_move('x')){
			hero[1]->location.x += hero[1]->motion.movement.x;
		}

		if (hero[1]->can_move('y')){
			hero[1]->location.y += hero[1]->motion.movement.y;
		}
	}

	for (int i=0; i<COINS; i++){
		if (coins[i] != NULL){
			if (hero[0]->intersects(coins[i])){
				delete coins[i];
				coins[i] = NULL;
			}
			if (hero[1]->intersects(coins[i])){
				delete coins[i];
				coins[i] = NULL;
			}
		}
	}
}

void Game::move_background()
{
	if (hero[0]->moving()){
		if (!hero[0]->can_move('x')){
			if (hero[0]->motion.movement.x < 0){
				hero[0]->location.x -= hero[0]->motion.movement.x;
				background.movement.x = -Sprite::step;
				background.active = 1;
			}
			else if (hero[0]->motion.movement.x > 0){
				hero[0]->location.x -= hero[0]->motion.movement.x;
				background.movement.x = Sprite::step;
				background.active = 1;
			}
		}

		if (!hero[0]->can_move('y')){
			if (hero[0]->motion.movement.y < 0){
				hero[0]->location.y -= hero[0]->motion.movement.y;
				background.movement.y = -Sprite::step;
				background.active = 1;
			}
			else if (hero[0]->motion.movement.y > 0){
				hero[0]->location.y -= hero[0]->motion.movement.y;
				background.movement.y = Sprite::step;
				background.active = 1;
			}
		}
	}
	else{
		background.movement.x = 0;
		background.movement.y = 0;
		background.active = 0;
	}

	// move grid
	if (background.active){
		grid_base.y += background.movement.y;
		grid_base.x += background.movement.x;
	}
}

void Game::process_inputs()
{
	int numkeys;
	const Uint8 *state = SDL_GetKeyboardState(&numkeys);
	last_input_state = input_state;
	input_state = 0;

	//hero[0]
	if (state[SDL_SCANCODE_W]){
		hero[0]->direction |= HERO_MOVE_UP;
	}
	else{
		hero[0]->direction &= ~(HERO_MOVE_UP);
	}
	if (state[SDL_SCANCODE_A]){
		hero[0]->direction |= HERO_MOVE_LEFT;
	}
	else{
		hero[0]->direction &= ~(HERO_MOVE_LEFT);
	}
	if (state[SDL_SCANCODE_S]){
		hero[0]->direction |= HERO_MOVE_DOWN;
	}
	else{
		hero[0]->direction &= ~(HERO_MOVE_DOWN);
	}
	if (state[SDL_SCANCODE_D]){
		hero[0]->direction |= HERO_MOVE_RIGHT;
	}
	else{
		hero[0]->direction &= ~(HERO_MOVE_RIGHT);
	}
	if (state[SDL_SCANCODE_SPACE]){
		hero[0]->action |= HERO_ACTION_COIN;
	}
	else{
		hero[0]->action &= ~(HERO_ACTION_COIN);
	}

	//hero[1]
	if (state[SDL_SCANCODE_UP]){
		hero[1]->direction |= HERO_MOVE_UP;
	}
	else{
		hero[1]->direction &= ~(HERO_MOVE_UP);
	}
	if (state[SDL_SCANCODE_LEFT]){
		hero[1]->direction |= HERO_MOVE_LEFT;
	}
	else{
		hero[1]->direction &= ~(HERO_MOVE_LEFT);
	}
	if (state[SDL_SCANCODE_DOWN]){
		hero[1]->direction |= HERO_MOVE_DOWN;
	}
	else{
		hero[1]->direction &= ~(HERO_MOVE_DOWN);
	}
	if (state[SDL_SCANCODE_RIGHT]){
		hero[1]->direction |= HERO_MOVE_RIGHT;
	}
	else{
		hero[1]->direction &= ~(HERO_MOVE_RIGHT);
	}
	if (state[SDL_SCANCODE_KP_ENTER]){
		hero[1]->action |= HERO_ACTION_COIN;
	}
	else{
		hero[1]->action &= ~(HERO_ACTION_COIN);
	}

	//globals
	if (state[SDL_SCANCODE_ESCAPE] || state[SDL_SCANCODE_Q]){
		input_state |= INPUT_STATE_QUIT;
	}

	//joysticks
	if (use_joy){
		for (int i=0; i<MAX_JOYDEV; i++){
			if (joy[i] != NULL){
				Sint16 x_move = SDL_JoystickGetAxis(joy[i], 0);
				if (x_move > 0){
					hero[0]->direction |= HERO_MOVE_RIGHT;
				}
				else if (x_move < 0){
					hero[0]->direction |= HERO_MOVE_LEFT;
				}
				else{
					hero[0]->direction = 0;
				}
			}
		}

		Uint8 button[16] = {};
		for (int i=0; i<MAX_JOYDEV; i++){
			if (joy[i] != NULL){
				for (int j=0; j<SDL_JoystickNumButtons(joy[i]); j++){
					button[j] = SDL_JoystickGetButton(joy[i], j);
					/*
					if (button[j]){
						printf("button %d on joy %d is pressed\n", j, i);
					}
					*/
				}

				if (button[0]){
					hero[0]->action |= HERO_ACTION_COIN;
				}
				else{
					hero[0]->action &= ~(HERO_ACTION_COIN);
				}
			}
		}
	}

	process_state();
}

void Game::process_state()
{
	if (input_state & INPUT_STATE_QUIT){
		running = 0;
	}

	process_hero_state(0);
	process_hero_state(1);
}

void Game::process_hero_state(int heronum)
{

	hero[heronum]->motion.active = 0;
	hero[heronum]->motion.movement.y = 0;
	hero[heronum]->motion.movement.x = 0;

	if (hero[heronum]->direction & HERO_MOVE_UP){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.y -= Sprite::step;
	}

	if (hero[heronum]->direction & HERO_MOVE_DOWN){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.y += Sprite::step;
	}

	if (hero[heronum]->direction & HERO_MOVE_LEFT){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.x -= Sprite::step;
	}

	if (hero[heronum]->direction & HERO_MOVE_RIGHT){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.x += Sprite::step;
	}

	if (hero[heronum]->action & HERO_ACTION_COIN){
		int new_x = hero[heronum]->location.x;
		if (hero[heronum]->motion.movement.x < 0){
			new_x += (hero[heronum]->width / 2) + 10;
		}
		else{
			new_x -= (hero[heronum]->width / 2) + 10;
		}
		add_coin(new_x, hero[heronum]->location.y + 15);
	}
}

void Game::process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				running = 0;
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
				switch (event.window.event){
					case SDL_WINDOWEVENT_RESIZED:
					{
						//TODO: this doesn't really work yet
						/*
						width = event.window.data1;
						height = event.window.data2;
						create_pixels();
						*/
						//fprintf(stdout, "window resized to %i, %i\n", event.window.data1, event.window.data2);
					} break;
					default:
					{
						//fprintf(stdout, "sdl window event: %i\n", event.window.event);
					} break;
				}
			} break;
			default:
			{
				fprintf(stdout, "sdl event: %x\n", event.type);
			} break;
		}
	}
}


/*
			case SDL_KEYDOWN:
			{
				if (event.key.repeat){
					break;
				}
				switch (event.key.keysym.sym){
					case SDLK_b:
					{
						sound.play(Sound::SOUND_BEEP);
					} break;
					case SDLK_w:
					{
						hero[0]->motion.active = 1;
						hero[0]->motion.movement.y = -Sprite::step;
					} break;
					case SDLK_s:
					{
						hero[0]->motion.active = 1;
						hero[0]->motion.movement.y = Sprite::step;
					} break;
					case SDLK_a:
					{
						hero[0]->motion.active = 1;
						hero[0]->motion.movement.x = -Sprite::step;
					} break;
					case SDLK_d:
					{
						hero[0]->motion.active = 1;
						hero[0]->motion.movement.x = Sprite::step;
					} break;
					case SDLK_LEFT:
					{
						background.active = 1;
						background.movement.x = 1;
					} break;
					case SDLK_RIGHT:
					{
						background.active = 1;
						background.movement.x = -1;
					} break;
					case SDLK_UP:
					{
						background.active = 1;
						background.movement.y = -1;
					} break;
					case SDLK_DOWN:
					{
						background.active = 1;
						background.movement.y = 1;
					} break;
					case SDLK_SPACE:
					{
						int new_x = hero[0]->location.x;
						if (hero[0]->motion.movement.x < 0){
							new_x += (hero[0]->width / 2) + 10;
						}
						else{
							new_x -= (hero[0]->width / 2) + 10;
						}
						add_coin(new_x, hero[0]->location.y + 15);
					} break;
					case SDLK_q:
					case SDLK_ESCAPE:
					{
						running = 0;
					} break;
				}
			} break;
			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym){
					case SDLK_w:
					case SDLK_s:
					{
						hero[0]->motion.active = 1;
						hero[0]->motion.movement.y = 0;
						if (!hero[0]->motion.movement.x && !hero[0]->motion.movement.y){
							hero[0]->motion.active = 0;
						}
					} break;
					case SDLK_a:
					case SDLK_d:
					{
						hero[0]->motion.movement.x = 0;
						if (!hero[0]->motion.movement.x && !hero[0]->motion.movement.y){
							hero[0]->motion.active = 0;
						}
					} break;
					case SDLK_RIGHT:
					case SDLK_LEFT:
					{
						background.active = 0;
						background.movement.x = 0;
					} break;
					case SDLK_UP:
					case SDLK_DOWN:
					{
						background.active = 0;
						background.movement.y = 0;
					} break;
				}
			} break;
*/
