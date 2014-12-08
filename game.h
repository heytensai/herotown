#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "point.h"
#include "video.h"
#include "sprite.h"
#include "hero.h"
#include "sound.h"
#include "input.h"
#include "SDL2/SDL_ttf.h"

class Game
{
private:
	static const int BLOCKS = 128;
	static const int COINS = 256;
	static const int COIN_TICKS = 800;
	static const int MAX_JOYDEV = 4;
	int width;
	int height;
	Hero *hero[2];
	point_t grid_base;
	movement_t background;
	Sprite *blocks[BLOCKS];
	Sprite *coins[COINS];
	input_state_t last_input_state;
	input_state_t input_state;
	Uint32 last_coin_added;
	bool use_joy;
	SDL_Joystick *joy[MAX_JOYDEV]; // 4 joydevs seems enough
	TTF_Font *font;

	void init_blocks();
	void init_coins();
	void init_hero();
	void init_controller();
	void init_font();
	void add_coin(int x, int y, bool ignore_tick);
	void process_hero_state(int heronum);
	void process_state();
	void render_score(int heronum);

public:
	int running;
	Video *video;
	Sound sound;

	Game(int width, int height);
	~Game();
	void process_events();
	void process_inputs();
	void move_background();
	void move_hero();
	void render();
	void create_random_coin();
};

#endif /* GAME_H */
