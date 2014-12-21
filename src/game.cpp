#include "game.h"
#include <time.h>

Game::Game(Video *video)
{
	this->video = video;
	running = 1;
	input_state = 0;

	sound.init();
	//init_controller();

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

	video = NULL;
}

void Game::start()
{
	background.active = 0;
	background.movement.x = 0;
	background.movement.y = 0;

	init_hero();
	init_blocks();
	init_coins();

	memset(bombs, 0, sizeof(bombs));

	start_time = SDL_GetTicks() / 1000;
	running = 1;
}

void Game::end()
{
	if (hero[0] != NULL){
		delete hero[0];
		hero[0] = NULL;
	}
	if (hero[1] != NULL){
		delete hero[1];
		hero[1] = NULL;
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

void Game::add_bomb(Hero *owner, int x, int y)
{
	Uint32 cur_tick = SDL_GetTicks();
	if (cur_tick - owner->last_bomb_added < BOMB_TICKS){
		return;
	}
	owner->last_bomb_added = cur_tick;
	for (int i=0; i<BOMBS; i++){
		if (bombs[i] == NULL){
			bombs[i] = new Bomb(video);
			bombs[i]->location.x = x;
			bombs[i]->location.y = y;
			sound.play(Sound::SOUND_TICKING);

			return;
		}
	}
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
			coins[i] = new Coin(video);
			coins[i]->location.x = x;
			coins[i]->location.y = y;
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
	hero[0] = new Yoshi(video);
	hero[0]->location.x = 80;
	hero[0]->location.y = 520;

	hero[1] = new Mario(video);
	hero[1]->location.x = 720;
	hero[1]->location.y = 520;
}

void Game::init_blocks()
{
	for (int i=0; i<BLOCKS; i++){
		blocks[i] = NULL;
	}
	for (int i=0; (i*32)<=video->width; i++){
		blocks[i] = new Block(video);
		blocks[i]->location.x = i * 32;
		blocks[i]->location.y = 550;
	}
}

void Game::render()
{
	video->start_render();
	video->blit_background();
	for (int i=0; i<BLOCKS; i++){
		if (blocks[i] != NULL){
			blocks[i]->render();
		}
	}
	for (int i=0; i<COINS; i++){
		if (coins[i] != NULL){
			coins[i]->render();
		}
	}
	hero[0]->render();
	hero[1]->render();
	render_bombs();
	render_score(0);
	render_score(1);
	render_time();
	video->finish_render();
}

void Game::render_bombs()
{
	for (int i=0; i<BOMBS; i++){
		if (bombs[i] != NULL){
			bombs[i]->render();
		}
	}
}

void Game::render_time()
{
	char s[16];
	int now = SDL_GetTicks() / 1000;
	int elapsed = running_time - (now - start_time);
	int min = elapsed / 60;
	int sec = elapsed % 60;
	snprintf(s, sizeof(s), "%02d:%02d", min, sec);

	video->render_text(350, 30, s);
}

void Game::render_score(int heronum)
{
	char s[4];
	snprintf(s, 4, "%i", hero[heronum]->score);

	video->render_text(20 + (720 * heronum), 20, s);
}

void Game::move_hero(Hero *h)
{
	if (h->motion.active){
		if (h->can_move('x')){
			h->location.x += h->motion.movement.x;
		}

		if (h->can_move('y')){
			h->location.y += h->motion.movement.y;
		}
	}
}

void Game::move_heros()
{
	move_hero(hero[0]);
	move_hero(hero[1]);

	for (int i=0; i<COINS; i++){
		if (coins[i] != NULL){
			if (hero[0]->intersects(coins[i], 0)){
				sound.play(Sound::SOUND_DING1);
				hero[0]->score++;
				delete coins[i];
				coins[i] = NULL;
			}
			if (hero[1]->intersects(coins[i], 0)){
				sound.play(Sound::SOUND_DING2);
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
		hero[0]->action |= HERO_ACTION_BOMB;
	}
	else{
		hero[0]->action &= ~(HERO_ACTION_BOMB);
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
	if (state[SDL_SCANCODE_RETURN]){
		hero[1]->action |= HERO_ACTION_BOMB;
	}
	else{
		hero[1]->action &= ~(HERO_ACTION_BOMB);
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
					hero[0]->action |= HERO_ACTION_BOMB;
				}
				else{
					hero[0]->action &= ~(HERO_ACTION_BOMB);
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

	process_bombs();
}

void Game::process_bombs()
{
	for (int i=0; i<BOMBS; i++){
		if (bombs[i] != NULL){
			if (bombs[i]->ready_to_explode()){
				sound.play(Sound::SOUND_EXPLODE);
				bombs[i]->explode();

				if (bombs[i]->intersects(hero[0], Bomb::RANGE)){
					sound.play(Sound::SOUND_BEEP);
					hero[0]->subtract_coins(10);
				}
				if (bombs[i]->intersects(hero[1], Bomb::RANGE)){
					sound.play(Sound::SOUND_BEEP);
					hero[1]->subtract_coins(10);
				}
			}
			if (bombs[i]->exploded()){
				delete bombs[i];
				bombs[i] = NULL;
			}
		}
	}
}

bool Game::process_hero_movement_direction(int heronum, int move, int direction)
{
	int other_hero = 1;
	if (heronum == 1){
		other_hero = 0;
	}

	if (hero[heronum]->direction & move){
		hero[heronum]->motion.active = 1;
		if (hero[heronum]->intersects(hero[other_hero], Hero::STEP, direction)) return false;
		for (int i=0; i<BOMBS; i++){
			if (bombs[i] != NULL){
				if (hero[heronum]->intersects(bombs[i], Hero::STEP, direction)) return false;
			}
		}
		return true;
	}
	
	return false;
}

void Game::process_hero_state(int heronum)
{
	hero[heronum]->motion.active = 0;
	hero[heronum]->motion.movement.y = 0;
	hero[heronum]->motion.movement.x = 0;

	if (process_hero_movement_direction(heronum, HERO_MOVE_UP, Sprite::DIRECTION_UP)){
		hero[heronum]->motion.movement.y -= Hero::STEP;
		hero[heronum]->set_animation(Animation::WALK_LEFT);
	}
	if (process_hero_movement_direction(heronum, HERO_MOVE_DOWN, Sprite::DIRECTION_DOWN)){
		hero[heronum]->motion.movement.y += Hero::STEP;
		hero[heronum]->set_animation(Animation::WALK_RIGHT);
	}
	if (process_hero_movement_direction(heronum, HERO_MOVE_RIGHT, Sprite::DIRECTION_RIGHT)){
		hero[heronum]->motion.movement.x += Hero::STEP;
		hero[heronum]->set_animation(Animation::WALK_RIGHT);
	}
	if (process_hero_movement_direction(heronum, HERO_MOVE_LEFT, Sprite::DIRECTION_LEFT)){
		hero[heronum]->motion.movement.x -= Hero::STEP;
		hero[heronum]->set_animation(Animation::WALK_LEFT);
	}

	if (hero[heronum]->action & HERO_ACTION_BOMB){
		int new_x = hero[heronum]->location.x;
		if (hero[heronum]->motion.movement.x < 0){
			new_x += (hero[heronum]->width / 2) + 10;
		}
		else{
			new_x -= (hero[heronum]->width / 2) + 10;
		}
		add_bomb(hero[heronum], new_x, hero[heronum]->location.y + 15);
	}
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
	if (elapsed >= running_time){
		running = 0;
	}
}

/*
 * return true if we should exit
 */
bool Game::intro_screen()
{
	IntroMenu menu(video);
	menu.render();
	menu.event_loop();
	running_time = menu.timer_option;

	return menu.exit();
}

void Game::score_screen()
{
	ScoreMenu menu(video);
	menu.score1 = hero[0]->score;
	menu.score2 = hero[1]->score;
	menu.render();
	menu.event_loop();
}

void Game::main_loop()
{
	last_tick = SDL_GetTicks();
	while (running){
		current_tick = SDL_GetTicks();

		// process events
		process_inputs();
		process_events();

		create_random_coin();
		move_heros();
		render();

		last_tick = SDL_GetTicks();
		Uint32 ticker = last_tick - current_tick;
		if (ticker < FRAME_TICKS){
			Uint32 sleep_ticks = FRAME_TICKS - ticker;
			//printf("render took %dms, sleeping %dms\n", ticker, sleep_ticks);
			SDL_Delay(sleep_ticks);
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
