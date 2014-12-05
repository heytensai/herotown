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
}

Game::~Game()
{
	if (hero != NULL){
		delete hero;
		hero = NULL;
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
}

void Game::add_coin(int x, int y)
{
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
		add_coin(i * 30 + 200, 530);
	}
}

void Game::init_hero()
{
	hero = new Sprite(48, 64);
	hero->location.x = 80;
	hero->location.y = 520;

	hero->motion.active = 0;
	hero->motion.movement.x = 0;
	hero->motion.movement.y = 0;

	hero->enable_animation(8);
	hero->load_animation(video->renderer, "resources/yoshi0.png");
	hero->load_animation(video->renderer, "resources/yoshi1.png");
	hero->load_animation(video->renderer, "resources/yoshi2.png");
	hero->load_animation(video->renderer, "resources/yoshi3.png");
	hero->load_animation(video->renderer, "resources/yoshi4.png");
	hero->load_animation(video->renderer, "resources/yoshi5.png");
	hero->load_animation(video->renderer, "resources/yoshi6.png");
	hero->load_animation(video->renderer, "resources/yoshi7.png");

	// this must happen after video is created
	hero->load_image(video->renderer, "resources/yoshi0.png");
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
	hero->render(video->renderer);
	video->finish_render();
}

void Game::move_hero()
{
	// move hero (if hero is in bounds)
	if (hero->motion.active){
		if (hero->can_move('x')){
			hero->location.x += hero->motion.movement.x;
		}

		if (hero->can_move('y')){
			hero->location.y += hero->motion.movement.y;
		}
	}

	for (int i=0; i<COINS; i++){
		if (coins[i] != NULL){
			if (hero->intersects(coins[i])){
				delete coins[i];
				coins[i] = NULL;
			}
		}
	}
}

void Game::move_background()
{
	if (hero->moving()){
		if (!hero->can_move('x')){
			if (hero->motion.movement.x < 0){
				hero->location.x -= hero->motion.movement.x;
				background.movement.x = -Sprite::step;
				background.active = 1;
			}
			else if (hero->motion.movement.x > 0){
				hero->location.x -= hero->motion.movement.x;
				background.movement.x = Sprite::step;
				background.active = 1;
			}
		}

		if (!hero->can_move('y')){
			if (hero->motion.movement.y < 0){
				hero->location.y -= hero->motion.movement.y;
				background.movement.y = -Sprite::step;
				background.active = 1;
			}
			else if (hero->motion.movement.y > 0){
				hero->location.y -= hero->motion.movement.y;
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
	if (state[SDL_SCANCODE_W]){
		input_state |= INPUT_STATE_MOVE_UP;
	}
	if (state[SDL_SCANCODE_A]){
		input_state |= INPUT_STATE_MOVE_LEFT;
	}
	if (state[SDL_SCANCODE_S]){
		input_state |= INPUT_STATE_MOVE_DOWN;
	}
	if (state[SDL_SCANCODE_D]){
		input_state |= INPUT_STATE_MOVE_RIGHT;
	}
	if (state[SDL_SCANCODE_ESCAPE] || state[SDL_SCANCODE_Q]){
		input_state |= INPUT_STATE_QUIT;
	}

	process_state();
}

void Game::process_state()
{
	if (input_state & INPUT_STATE_QUIT){
		running = 0;
	}

	hero->motion.active = 0;
	hero->motion.movement.y = 0;
	hero->motion.movement.x = 0;

	if (input_state & INPUT_STATE_MOVE_UP){
		hero->motion.active = 1;
		hero->motion.movement.y -= Sprite::step;
	}

	if (input_state & INPUT_STATE_MOVE_DOWN){
		hero->motion.active = 1;
		hero->motion.movement.y += Sprite::step;
	}

	if (input_state & INPUT_STATE_MOVE_LEFT){
		hero->motion.active = 1;
		hero->motion.movement.x -= Sprite::step;
	}

	if (input_state & INPUT_STATE_MOVE_RIGHT){
		hero->motion.active = 1;
		hero->motion.movement.x += Sprite::step;
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
						hero->motion.active = 1;
						hero->motion.movement.y = -Sprite::step;
					} break;
					case SDLK_s:
					{
						hero->motion.active = 1;
						hero->motion.movement.y = Sprite::step;
					} break;
					case SDLK_a:
					{
						hero->motion.active = 1;
						hero->motion.movement.x = -Sprite::step;
					} break;
					case SDLK_d:
					{
						hero->motion.active = 1;
						hero->motion.movement.x = Sprite::step;
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
						int new_x = hero->location.x;
						if (hero->motion.movement.x < 0){
							new_x += (hero->width / 2) + 10;
						}
						else{
							new_x -= (hero->width / 2) + 10;
						}
						add_coin(new_x, hero->location.y + 15);
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
						hero->motion.active = 1;
						hero->motion.movement.y = 0;
						if (!hero->motion.movement.x && !hero->motion.movement.y){
							hero->motion.active = 0;
						}
					} break;
					case SDLK_a:
					case SDLK_d:
					{
						hero->motion.movement.x = 0;
						if (!hero->motion.movement.x && !hero->motion.movement.y){
							hero->motion.active = 0;
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
