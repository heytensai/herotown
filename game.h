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

extern int width;
extern int height;
extern int pitch;

static inline int pitch_offset(int x, int y)
{
	return (x + (y * pitch));
}

class Game
{
private:

public:
	Sprite sprite;
	int running;
	Video video;
	Sound sound;
	point_t grid_base;
	movement_t vertical;
	movement_t horizontal;

	Game();
	void process_events();
};

#endif /* GAME_H */
