#include "SDL.h"

#define WIDTH 640
#define HEIGHT 480
#define COLORDEPTH 24

static SDL_Surface *screen = NULL;
static SDL_Surface *back_buffer = NULL;
static int global_running = 1;

static void video_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
}

static void create_buffer()
{
	if (back_buffer){
		SDL_FreeSurface(back_buffer);
		back_buffer = NULL;
	}
	back_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, COLORDEPTH, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
}

static void create_window()
{
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, COLORDEPTH, 0);
	if (screen == NULL){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}
}

int main(int argc, char **argv)
{
	video_init();
	create_window();
	create_buffer();
}
