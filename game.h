#ifndef GAME_H
#define GAME_H

#define APPNAME "sdlapp"
#define WIDTH 800
#define HEIGHT 600
#define COLORDEPTH 32
#define BPP (COLORDEPTH / 4)

#include "point.h"
#include "video.h"
#include "sprite.h"
#include "sound.h"
#include "input.h"

class Game
{
private:
	static const int BLOCKS = 128;
	static const int COINS = 128;
	static const int COIN_TICKS = 200;
	static const int MAX_JOYDEV = 4;
	int width;
	int height;
	Sprite *hero;
	point_t grid_base;
	movement_t background;
	Sprite *blocks[BLOCKS];
	Sprite *coins[COINS];
	input_state_t last_input_state;
	input_state_t input_state;
	Uint32 last_coin_added;
	bool use_joy;
	SDL_Joystick *joy[MAX_JOYDEV]; // 4 joydevs seems enough

	void init_blocks();
	void init_coins();
	void init_hero();
	void init_controller();
	void add_coin(int x, int y, bool ignore_tick);
	void process_state();

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
};

#endif /* GAME_H */
