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

public:
	int width;
	int height;
	Sprite sprite;
	int running;
	Video *video;
	Sound sound;
	point_t grid_base;
	movement_t background;

	Game(int width, int height);
	void process_events();
};

#endif /* GAME_H */
