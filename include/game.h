#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "point.h"
#include "video.h"
#include "sprite.h"
#include "hero.h"
#include "sound.h"
#include "input.h"
#include "bomb.h"
#include "menu.h"

class Game
{
private:
	static const int BLOCKS = 128;
	static const int COINS = 256;
	static const int BOMBS = 6;
	static const int COIN_TICKS = 800;
	static const int BOMB_TICKS = 1000;
	static const int MAX_JOYDEV = 4;
	int width;
	int height;
	Hero *hero[2];
	point_t grid_base;
	movement_t background;
	Block *blocks[BLOCKS];
	Coin *coins[COINS];
	Bomb *bombs[BOMBS];
	input_state_t last_input_state;
	input_state_t input_state;
	Uint32 last_coin_added;
	Uint32 last_bomb_added;
	bool use_joy;
	SDL_Joystick *joy[MAX_JOYDEV]; // 4 joydevs seems enough
	int start_time;
	int running_time;

	void init_blocks();
	void init_coins();
	void init_hero();
	void init_controller();
	void add_coin(int x, int y, bool ignore_tick);
	void add_bomb(int x, int y);
	void process_hero_state(int heronum);
	bool process_hero_movement_direction(int heronum, int move, int direction);
	void process_state();
	void process_bombs();
	void render_score(int heronum);
	void render_time();
	void render_bombs();
	void move_hero(Hero *hero);

public:
	int running;
	bool exit_requested;
	Video *video;
	Sound sound;

	Game(int width, int height);
	~Game();
	void process_events();
	void process_inputs();
	void move_background();
	void move_heros();
	void render();
	void create_random_coin();
	void start();
	void end();
	bool intro_screen();
};

#endif /* GAME_H */