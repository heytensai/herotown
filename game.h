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

extern point_t grid_base;
extern movement_t vertical;
extern movement_t horizontal;

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

	Game();
	void process_events();
};

#endif /* GAME_H */
