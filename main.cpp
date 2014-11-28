#include "SDL.h"

#define WIDTH 640
#define HEIGHT 480
#define COLORDEPTH 24

static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

static SDL_Surface *screen = NULL;
static int global_running = 1;
static int x = 0;
static int y = 0;

static void video_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
}

static void create_window()
{
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, COLORDEPTH, SDL_HWSURFACE|SDL_RESIZABLE);
	if (screen == NULL){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
}

typedef struct {
	int active;
	int direction;
	int step;
} movement_t;

int main(int argc, char **argv)
{
	video_init();
	create_window();

	SDL_Event event;

	movement_t vertical;
	vertical.active = 0;
	vertical.direction = 0;
	vertical.step = 1;

	movement_t horizontal;
	horizontal.active = 1;
	horizontal.direction = 1;
	horizontal.step = 1;

	while (global_running){

		// process events
		while (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_QUIT:
				{
					global_running = 0;
				} break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym){
						case SDLK_LEFT:
						{
							horizontal.step++;
							fprintf(stdout, "horizontal step: %i\n", horizontal.step);
						} break;
						case SDLK_RIGHT:
						{
							horizontal.step--;
							fprintf(stdout, "horizontal step: %i\n", horizontal.step);
						} break;
						case SDLK_UP:
						{
							vertical.active = 1;
							vertical.direction = 0;
						} break;
						case SDLK_DOWN:
						{
							vertical.active = 1;
							vertical.direction = 1;
						} break;
						case SDLK_SPACE:
						{
							horizontal.active = !horizontal.active;
						} break;
						case SDLK_q:
						case SDLK_ESCAPE:
						{
							global_running = 0;
						} break;
					}
				} break;
				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym){
						case SDLK_UP:
						case SDLK_DOWN:
						{
							vertical.active = 0;
						} break;
					}
				} break;
				default:
				{
					//fprintf(stdout, "sdl event: %i\n", event.type);
				} break;
			}
		}

		// game loop
		if (vertical.active){
			if (vertical.direction){
				y += vertical.step;
			}
			else{
				y -= vertical.step;
			}
		}

		if (horizontal.active){
			if (horizontal.direction){
				x += horizontal.step;
			}
			else{
				x -= horizontal.step;
			}
		}

		// draw video
		if (SDL_MUSTLOCK(screen)){
			SDL_LockSurface(screen);
		}
		Uint32 *pixels;
		pixels = (Uint32 *)screen->pixels;
		for (int _x=0; _x<WIDTH; _x++){
			for (int _y=0; _y<HEIGHT; _y++){
				putpixel(screen, _x, _y, SDL_MapRGB(screen->format, 0x00, ((_x + x) & 0xff), ((_y + y) & 0xff)));
			}
		}
		if (SDL_MUSTLOCK(screen)){
			SDL_UnlockSurface(screen);
		}
		SDL_Flip(screen);
	}
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
