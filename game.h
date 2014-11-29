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

extern int global_running;
extern int width;
extern int height;
extern int pitch;

extern Video video;
extern point_t grid_base;
extern movement_t vertical;
extern movement_t horizontal;
extern Sprite sprite;

static inline int pitch_offset(int x, int y)
{
	return (x + (y * pitch));
}

#endif /* GAME_H */
