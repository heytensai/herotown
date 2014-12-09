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
	int start_time;

	void init_blocks();
	void init_coins();
	void init_hero();
	void init_controller();
	void init_font();
	void add_coin(int x, int y, bool ignore_tick);
	void process_hero_state(int heronum);
	bool process_hero_movement(Sprite *hero, Sprite *other, int direction, int *x_or_y, int step);
	void process_state();
	void render_score(int heronum);
	void render_time();
	void render_text(int x, int y, const char *text);
	void intro_screen_events();
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
	void intro_screen();
};

#endif /* GAME_H */
