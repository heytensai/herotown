#ifndef GAME_H
#define GAME_H

#define APPNAME "sdlapp"
#define WIDTH 800
#define HEIGHT 600
#define COLORDEPTH 32
#define BPP (COLORDEPTH / 4)

static int global_running = 1;
static int width = WIDTH;
static int height = HEIGHT;
static int pitch = width * 2;

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	int active;
	point_t movement;
} movement_t;

#endif /* GAME_H */
