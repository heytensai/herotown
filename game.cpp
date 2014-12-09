#include "game.h"
#include <time.h>

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

	sound.init();
	init_controller();
	init_font();

	srand(time(NULL));

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
}

Game::~Game()
{
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

	if (video != NULL){
		delete video;
		video = NULL;
	}
	if (font != NULL){
		TTF_CloseFont(font);
		font = NULL;
	}
	TTF_Quit();
}

void Game::intro_screen()
{
	exit_requested = false;
	running = 1;
	const char *s = "Press SPACE to begin";

	video->start_render();
	video->blit_background();
	render_text(200, 300, s);
	video->finish_render();

	while (running){
		intro_screen_events();
	}
}

void Game::intro_screen_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				running = 0;
				exit_requested = true;
			} break;
			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
					{
						running = 0;
						exit_requested = true;
					} break;
					case SDLK_SPACE:
					{
						running = 0;
					} break;
				}
			} break;
		}
	}
}

void Game::start()
{
	background.active = 0;
	background.movement.x = 0;
	background.movement.y = 0;

	init_hero();
	init_blocks();
	init_coins();

	start_time = SDL_GetTicks() / 1000;
	running = 1;
}

void Game::end()
{
	printf("hero 1 score %d\nhero 2 score %d\n", hero[0]->score, hero[1]->score);
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
}

void Game::init_font()
{
	TTF_Init();
	font = TTF_OpenFont("resources/DejaVuSans-Bold.ttf", 30);
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

void Game::create_random_coin()
{
	Uint32 cur_tick = SDL_GetTicks();
	if (cur_tick - last_coin_added < COIN_TICKS){
		return;
	}
	last_coin_added = cur_tick;

	int x, y;
	x = (rand() % (WIDTH - 40)) + 20;
	y = (rand() % (HEIGHT - 40)) + 20;
	add_coin(x, y, true);
	x = (rand() % (WIDTH - 40)) + 20;
	y = (rand() % (HEIGHT - 40)) + 20;
	add_coin(x, y, true);
	x = (rand() % (WIDTH - 40)) + 20;
	y = (rand() % (HEIGHT - 40)) + 20;
	add_coin(x, y, true);
}

void Game::init_coins()
{
	memset(coins, 0, sizeof(Sprite *) * COINS);
	for (int i=0; i<15; i++){
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

	hero[0]->enable_animation_right(8);
	hero[0]->load_animation_right(video->renderer, "resources/yoshi0.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi1.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi2.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi3.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi4.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi5.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi6.png");
	hero[0]->load_animation_right(video->renderer, "resources/yoshi7.png");
	hero[0]->enable_animation_left(8);
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left0.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left1.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left2.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left3.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left4.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left5.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left6.png");
	hero[0]->load_animation_left(video->renderer, "resources/yoshi-left7.png");

	hero[0]->load_image(video->renderer, "resources/yoshi0.png");

	hero[1] = new Hero(48, 64);
	hero[1]->location.x = 720;
	hero[1]->location.y = 520;

	hero[1]->motion.active = 0;
	hero[1]->motion.movement.x = 0;
	hero[1]->motion.movement.y = 0;

	hero[1]->enable_animation_right(8);
	hero[1]->load_animation_right(video->renderer, "resources/mario0.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario1.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario2.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario3.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario4.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario5.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario6.png");
	hero[1]->load_animation_right(video->renderer, "resources/mario7.png");
	hero[1]->enable_animation_left(8);
	hero[1]->load_animation_left(video->renderer, "resources/mario-left0.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left1.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left2.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left3.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left4.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left5.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left6.png");
	hero[1]->load_animation_left(video->renderer, "resources/mario-left7.png");

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
	render_score(0);
	render_score(1);
	render_time();
	video->finish_render();
}

void Game::render_time()
{
	char s[16];
	int now = SDL_GetTicks() / 1000;
	int elapsed = now - start_time;
	int min = elapsed / 60;
	int sec = elapsed % 60;
	snprintf(s, sizeof(s), "%02d:%02d", min, sec);

	render_text(350, 30, s);
}

void Game::render_text(int x, int y, const char *text)
{
	SDL_Color color = {0, 0, 0};
	SDL_Surface *f = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Texture *t = SDL_CreateTextureFromSurface(video->renderer, f);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = f->w;
	src.h = f->h;
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = f->w;
	dst.h = f->h;
	SDL_RenderCopy(video->renderer, t, &src, &dst);

	SDL_FreeSurface(f);
	SDL_DestroyTexture(t);
}

void Game::render_score(int heronum)
{
	SDL_Color color = {0, 0, 0};
	char s[4];
	snprintf(s, 4, "%i", hero[heronum]->score);
	SDL_Surface *f = TTF_RenderUTF8_Blended(font, s, color);
	SDL_Texture *t = SDL_CreateTextureFromSurface(video->renderer, f);

	render_text(20 + (720 * heronum), 20, s);
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
				hero[0]->score++;
				delete coins[i];
				coins[i] = NULL;
			}
			if (hero[1]->intersects(coins[i])){
				hero[1]->score++;
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
	if (state[SDL_SCANCODE_BACKSPACE]){
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

bool Game::process_hero_movement(Sprite *hero, Sprite *other, int direction, int *x_or_y, int step)
{
	if (hero->intersects(other, direction)){
		(*x_or_y) -= step;
	}
}

void Game::process_hero_state(int heronum)
{
	int other_hero = 1;
	if (heronum == 1){
		other_hero = 0;
	}

	hero[heronum]->motion.active = 0;
	hero[heronum]->motion.movement.y = 0;
	hero[heronum]->motion.movement.x = 0;

	if (hero[heronum]->direction & HERO_MOVE_UP){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.y -= Sprite::step;
		process_hero_movement(hero[heronum],
			hero[other_hero],
			Sprite::DIRECTION_UP,
			&hero[heronum]->motion.movement.y,
			-Sprite::step
		);
	}

	if (hero[heronum]->direction & HERO_MOVE_DOWN){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.y += Sprite::step;
		process_hero_movement(hero[heronum],
			hero[other_hero],
			Sprite::DIRECTION_DOWN,
			&hero[heronum]->motion.movement.y,
			Sprite::step
		);
	}

	if (hero[heronum]->direction & HERO_MOVE_LEFT){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.x -= Sprite::step;
		process_hero_movement(hero[heronum],
			hero[other_hero],
			Sprite::DIRECTION_LEFT,
			&hero[heronum]->motion.movement.x,
			-Sprite::step
		);
	}

	if (hero[heronum]->direction & HERO_MOVE_RIGHT){
		hero[heronum]->motion.active = 1;
		hero[heronum]->motion.movement.x = Sprite::step;
		process_hero_movement(hero[heronum],
			hero[other_hero],
			Sprite::DIRECTION_RIGHT,
			&hero[heronum]->motion.movement.x,
			Sprite::step
		);
	}

	/*
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
	*/
}

void Game::process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				exit_requested = true;
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

	int now = SDL_GetTicks() / 1000;
	int elapsed = now - start_time;
	if (elapsed >= 60){
		running = 0;
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
