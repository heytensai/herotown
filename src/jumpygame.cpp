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

void JumpyGame::add_block(int x, int y)
{
	for (int i=0; i<BLOCKS; i++){
		if (blocks[i] == NULL){
			blocks[i] = new Block(video);
			blocks[i]->location.x = x;
			blocks[i]->location.y = y;
			return;
		}
	}
}

void JumpyGame::init_blocks()
{
	for (int i=0; i<BLOCKS; i++){
		blocks[i] = NULL;
	}
	//floor
	for (int i=0; (i*32)<=video->width; i++){
		add_block(i * 32, 540);
	}

	add_block(0, 400);
	add_block(32, 400);
	add_block(64, 400);
	add_block(96, 400);
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
	for (int i=0; i<BLOCKS; i++){
		if (blocks[i] != NULL){
			delete blocks[i];
			blocks[i] = NULL;
		}
	}
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

void JumpyGame::process_hero_inputs(const Uint8 *state, int heronum)
{
	//reset hero
	hero[heronum]->motion.movement.x = 0;

	int run = 1;
	if (state[hero[heronum]->input_map[Hero::SPEED]]){
		run = 2;
	}

	if (state[hero[heronum]->input_map[Hero::JUMP]]){
		//somehow need to know if hero is on the ground
		if (hero[heronum]->can_jump){
			hero[heronum]->can_jump = false;
			hero[heronum]->velocity.direction = 0;
			hero[heronum]->velocity.speed = 30;
		}
	}
	if (state[hero[heronum]->input_map[Hero::LEFT]]){
		hero[heronum]->motion.movement.x = -Sprite::step * run;
		hero[heronum]->direction = Sprite::LEFT;
		hero[heronum]->facing = Sprite::LEFT;
	}
	if (state[hero[heronum]->input_map[Hero::RIGHT]]){
		hero[heronum]->motion.movement.x = Sprite::step * run;
		hero[heronum]->direction = Sprite::RIGHT;
		hero[heronum]->facing = Sprite::RIGHT;
	}
	if (state[hero[heronum]->input_map[Hero::BOMB]]){
		if (hero[heronum]->bomb == NULL){
			Bomb *b = new Bomb(video);
			hero[heronum]->bomb = b;
			int flop = 1;
			if (hero[heronum]->facing == Sprite::LEFT){
				flop = -1;
			}
			b->location.x = hero[heronum]->location.x + (flop * 30);
			b->location.y = hero[heronum]->location.y;
			b->motion.movement.x = (flop * 2);
			b->facing = Sprite::NONE;
		}
	}
	if (state[hero[heronum]->input_map[Hero::FIREBALL]]){
		hero[heronum]->add_fireball(video);
	}
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

	process_hero_inputs(state, 0);
	process_hero_inputs(state, 1);
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

	char score[4];
	snprintf(score, sizeof(score), "%d", hero[0]->health);
	video->render_text(10, 10, score);
	snprintf(score, sizeof(score), "%d", hero[1]->health);
	int offset = 30;
	if (hero[1]->health > 99){
		offset = 70;
	}
	else if (hero[1]->health > 9){
		offset = 50;
	}
	video->render_text(WIDTH - offset, 10, score);

	video->finish_render();
}

void JumpyGame::move_hero(int i)
{
	int other_hero = 0;
	if (i == 0){
		other_hero = 1;
	}

	// move my fireball too
	hero[i]->move_fireballs();

	// move my bomb too
	if (hero[i]->bomb){
		if (hero[i]->bomb->location.x > WIDTH || hero[i]->bomb->location.x < 0){
			delete hero[i]->bomb;
			hero[i]->bomb = NULL;
		}
		else{
			hero[i]->bomb->location.x += hero[i]->bomb->motion.movement.x;

			// apply velocity
			hero[i]->bomb->location.y -= hero[i]->bomb->velocity.speed;

			// adjust velocity for gravity
			hero[i]->bomb->velocity.speed -= gravity.speed;
			if (hero[i]->bomb->velocity.speed < Hero::TERMINAL_VELOCITY){
				hero[i]->bomb->velocity.speed = Hero::TERMINAL_VELOCITY;
			}

			// apply gravity
			hero[i]->bomb->location.y += gravity.speed;

			// hard coded floor
			if (hero[i]->bomb->location.y >= 300){
				hero[i]->bomb->location.y = 300;
			}
		}
	}

	//moving left
	if (hero[i]->motion.movement.x < 0){
		hero[i]->set_animation(Animation::WALK_LEFT);
		hero[i]->location.x += hero[i]->motion.movement.x;
		if (hero[i]->location.x < 0){
			hero[i]->location.x = 0;
		}
		//TODO check collision to the left
		if (hero[i]->intersects(hero[other_hero], 0)){
			while (hero[i]->intersects(hero[other_hero], 0)){
				hero[i]->location.x++;
			}
		}
	}
	//moving right
	else if (hero[i]->motion.movement.x > 0){
		hero[i]->set_animation(Animation::WALK_RIGHT);
		hero[i]->location.x += hero[i]->motion.movement.x;
		if (hero[i]->location.x > WIDTH){
			hero[i]->location.x = WIDTH;
		}
		//TODO check collision to the right 
		if (hero[i]->intersects(hero[other_hero], 0)){
			while (hero[i]->intersects(hero[other_hero], 0)){
				hero[i]->location.x--;
			}
		}
	}
	//going nowhere
	else{
		hero[i]->set_animation(Animation::NONE);
	}

	//going up
	if (hero[i]->velocity.speed > 0){
		for (int j=0; j<BLOCKS; j++){
			if (blocks[j] != NULL){
				while (hero[i]->intersects(blocks[j], hero[i]->velocity.speed, Sprite::UP)){
					hero[i]->velocity.speed--;
				}
			}
		}
	}

	// apply velocity
	hero[i]->location.y -= hero[i]->velocity.speed;

	// on a block?
	for (int j=0; j<BLOCKS; j++){
		if (blocks[j] != NULL){
			if (hero[i]->intersects(blocks[j], 0, Sprite::DOWN)){
				while (hero[i]->intersects(blocks[j], 0, Sprite::DOWN)){
					hero[i]->location.y--;
				}
				hero[i]->velocity.speed = 0;
				hero[i]->can_jump = true;
			}
		}
	}

	if (hero[i]->intersects(hero[other_hero], 0, Sprite::DOWN)){
		while (hero[i]->intersects(hero[other_hero], 0)){
			hero[i]->location.y--;
		}
		hero[i]->velocity.speed = 0;
		hero[i]->can_jump = true;
	}
	else{
		// adjust velocity for gravity
		hero[i]->velocity.speed -= gravity.speed;
		if (hero[i]->velocity.speed < Hero::TERMINAL_VELOCITY){
			hero[i]->velocity.speed = Hero::TERMINAL_VELOCITY;
		}

		// apply gravity
		hero[i]->location.y += gravity.speed;
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

