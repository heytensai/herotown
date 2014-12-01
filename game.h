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

class Game
{
private:
	static const int BLOCKS = 128;
	static const int COINS = 128;
	int width;
	int height;
	Sprite *hero;
	point_t grid_base;
	movement_t background;
	Sprite *blocks[BLOCKS];
	Sprite *coins[COINS];

	void init_blocks();
	void init_coins();
	void init_hero();
	void add_coin(int x, int y);

public:
	int running;
	Video *video;
	Sound sound;

	Game(int width, int height);
	void process_events();
	void move_background();
	void move_hero();
	void render();
};

#endif /* GAME_H */
