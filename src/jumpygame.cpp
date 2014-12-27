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
		add_block((i * 32) - 16, 540);
	}

	add_block(16, 380);
	add_block(48, 380);
	add_block(80, 380);
	add_block(112, 380);

	add_block(video->width-16, 380);
	add_block(video->width-48, 380);
	add_block(video->width-80, 380);
	add_block(video->width-112, 380);

	int middle = video->width/2;
	add_block(middle, 220);
	add_block(middle + 32, 220);
	add_block(middle + 64, 220);
	add_block(middle + 96, 220);
	add_block(middle + 128, 220);
	add_block(middle - 32, 220);
	add_block(middle - 64, 220);
	add_block(middle - 96, 220);
	add_block(middle - 128, 220);

	add_block(16, 110);
	add_block(48, 110);
	add_block(80, 110);
	add_block(112, 110);

	add_block(video->width-16, 110);
	add_block(video->width-48, 110);
	add_block(video->width-80, 110);
	add_block(video->width-112, 110);
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
		hero[heronum]->direction = Sprite::DIR_LEFT;
		hero[heronum]->facing = Sprite::DIR_LEFT;
	}
	if (state[hero[heronum]->input_map[Hero::RIGHT]]){
		hero[heronum]->motion.movement.x = Sprite::step * run;
		hero[heronum]->direction = Sprite::DIR_RIGHT;
		hero[heronum]->facing = Sprite::DIR_RIGHT;
	}
	if (state[hero[heronum]->input_map[Hero::BOMB]]){
		if (hero[heronum]->bomb == NULL){
			Bomb *b = new Bomb(video);
			hero[heronum]->bomb = b;
			int flop = 1;
			if (hero[heronum]->facing == Sprite::DIR_LEFT){
				flop = -1;
			}
			b->location.x = hero[heronum]->location.x + (flop * 50);
			b->location.y = hero[heronum]->location.y;
			b->motion.movement.x = (flop * 3);
			b->facing = Sprite::DIR_NONE;
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

void JumpyGame::move_bomb(Bomb *bomb)
{
	if (bomb == NULL){
		return;
	}

	bomb->location.x += bomb->motion.movement.x;

	// hit a player?
	if (!bomb->exploding()){
		if (bomb->intersects(hero[0], 0)){
			bomb->explode();
			hero[0]->health -= 10;
		}
		if (bomb->intersects(hero[1], 0)){
			if (!bomb->exploding()){
				bomb->explode();
			}
			hero[1]->health -= 10;
		}
	}

	// on a block?
	for (int j=0; j<BLOCKS; j++){
		if (blocks[j] != NULL){
			if (bomb->intersects(blocks[j], 1, Sprite::DIR_DOWN)){
				while (bomb->intersects(blocks[j], 0, Sprite::DIR_DOWN)){
					bomb->location.y--;
				}
				bomb->velocity.speed = 0;
				return;
			}
		}
	}

	// ignore gravity if we're exploding
	if (bomb->exploding()){
		return;
	}

	// apply velocity
	bomb->location.y -= bomb->velocity.speed;

	// adjust velocity for gravity
	bomb->velocity.speed -= gravity.speed;
	if (bomb->velocity.speed < Hero::TERMINAL_VELOCITY){
		bomb->velocity.speed = Hero::TERMINAL_VELOCITY;
	}

	// apply gravity
	bomb->location.y += gravity.speed;
}

void JumpyGame::move_fireballs(int heronum)
{
	int other_hero = 0;
	if (heronum == 0){
		other_hero = 1;
	}

	for (int i=0; i<Hero::FIREBALLS; i++){
		if (hero[heronum]->fireballs[i] != NULL){
			if (hero[heronum]->fireballs[i]->location.x > WIDTH || hero[heronum]->fireballs[i]->location.x < 0){
				delete hero[heronum]->fireballs[i];
				hero[heronum]->fireballs[i] = NULL;
			}
			else{
				hero[heronum]->fireballs[i]->location.x += hero[heronum]->fireballs[i]->motion.movement.x;

				if (hero[heronum]->fireballs[i]->intersects(hero[other_hero], 0)){
					hero[other_hero]->health -= 5;
					delete hero[heronum]->fireballs[i];
					hero[heronum]->fireballs[i] = NULL;
				}
				else{
					for (int j=0; j<BLOCKS; j++){
						if (blocks[j] != NULL){
							if (hero[heronum]->fireballs[i]->intersects(blocks[j], 0)){
								delete hero[heronum]->fireballs[i];
								hero[heronum]->fireballs[i] = NULL;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void JumpyGame::move_hero(int i)
{
	int other_hero = 0;
	if (i == 0){
		other_hero = 1;
	}

	// move my fireball too
	move_fireballs(i);

	// move my bomb too
	if (hero[i]->bomb){
		// delete if off screen
		if (hero[i]->bomb->location.x > WIDTH || hero[i]->bomb->location.x < 0 || hero[i]->bomb->location.y > video->height){
			delete hero[i]->bomb;
			hero[i]->bomb = NULL;
		}
		else{
			move_bomb(hero[i]->bomb);

			if (hero[i]->bomb->exploded()){
				delete hero[i]->bomb;
				hero[i]->bomb = NULL;
			}
		}

	}

	hero[i]->can_jump = false;

	//moving left
	if (hero[i]->motion.movement.x < 0){
		hero[i]->set_animation(Animation::NAME_WALK_LEFT);
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
		hero[i]->set_animation(Animation::NAME_WALK_RIGHT);
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
		hero[i]->set_animation(Animation::NAME_NONE);
	}

	//going up
	if (hero[i]->velocity.speed > 0){
		for (int j=0; j<BLOCKS; j++){
			if (blocks[j] != NULL){
				while (hero[i]->intersects(blocks[j], hero[i]->velocity.speed, Sprite::DIR_UP)){
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
			if (hero[i]->intersects(blocks[j], 0, Sprite::DIR_DOWN)){
				while (hero[i]->intersects(blocks[j], 0, Sprite::DIR_DOWN)){
					hero[i]->location.y--;
				}
				hero[i]->velocity.speed = 0;
				hero[i]->can_jump = true;
			}
		}
	}

	if (hero[i]->intersects(hero[other_hero], 0, Sprite::DIR_DOWN)){
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
	hero[1]->location.x = video->width - 30;
	hero[1]->location.y = 300;
	hero[1]->facing = Sprite::DIR_LEFT;
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

