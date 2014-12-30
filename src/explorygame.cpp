#include "explorygame.h"

ExploryGame::ExploryGame(Video *video, Sound *sound)
	: Game(video, sound)
{
	hero = NULL;
	running = true;
	tile = NULL;
}

ExploryGame::~ExploryGame()
{
	video = NULL;
}

void ExploryGame::start()
{
	running = true;
	exit_requested = false;
	hero = new Yoshi(video);
	hero->location.x = 200;
	hero->location.y = 200;

	tile = new Tile(video);
	load_tile_maps();
}

void ExploryGame::load_tile_maps()
{
	tile_map = new Uint32[TILE_X * TILE_Y];
	memset(tile_map, 0, sizeof(Uint32) * TILE_X * TILE_Y);

	for (int i=0; i<TILE_X; i++){
		tile_map[i] = 1;
		tile_map[i + ((TILE_Y-1) * TILE_X)] = 1;
	}
	for (int i=0; i<TILE_Y; i++){
		tile_map[TILE_X*i] = 1;
	}
}

void ExploryGame::end()
{
	if (hero != NULL){
		delete hero;
		hero = NULL;
	}
	if (tile != NULL){
		delete tile;
		tile = NULL;
	}
}

bool ExploryGame::intro_screen()
{
	return !running;
}

void ExploryGame::score_screen()
{
}

void ExploryGame::process_inputs()
{
	int numkeys;
	const Uint8 *state = SDL_GetKeyboardState(&numkeys);

	// global keys
	if (state[SDL_SCANCODE_ESCAPE]){
		running = false;
	}
	if (state[SDL_SCANCODE_TAB]){
		hero->location.x = 30;
		hero->location.y = 300;
		hero->velocity.speed = 0;
	}
	// emergency exit
	if (state[SDL_SCANCODE_BACKSPACE]){
		exit(0);
	}

	point_t new_loc = hero->location;
	//hero controls
	if (state[SDL_SCANCODE_W]){
		new_loc.y += -Sprite::step;
	}
	if (state[SDL_SCANCODE_A]){
		new_loc.x += -Sprite::step;
	}
	if (state[SDL_SCANCODE_S]){
		new_loc.y += Sprite::step;
	}
	if (state[SDL_SCANCODE_D]){
		new_loc.x += Sprite::step;
	}
	//TODO check new_loc for moveability
	hero->location = new_loc;

}

void ExploryGame::process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				exit_requested = true;
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
				switch (event.window.event){
					case SDL_WINDOWEVENT_RESIZED:
					{
					} break;
					default:
					{
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

void ExploryGame::update_world()
{
}

void ExploryGame::render_map()
{
	for (int x=0; x<TILE_X; x++){
		for (int y=0; y<TILE_Y; y++){
			if (tile_map[x + (y * TILE_X)]){
				tile->location.x = x * tile->width;
				tile->location.y = y * tile->height;
				tile->render();
			}
		}
	}
}

void ExploryGame::render()
{
	video->start_render();
	video->blit_background();
	render_map();
	hero->render();
	video->finish_render();
}

void ExploryGame::main_loop()
{
	while (running){
		// process events
		process_inputs();
		process_events();

		update_world();
		render();
	}
}

