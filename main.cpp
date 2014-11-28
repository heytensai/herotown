#include "SDL2/SDL.h"

#define APPNAME "sdlapp"
#define WIDTH 640
#define HEIGHT 480
#define COLORDEPTH 32
#define BPP (COLORDEPTH / 4)

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static Uint32 *pixels = NULL;
static int global_running = 1;
static int x = 0;
static int y = 0;
static int width = WIDTH;
static int height = HEIGHT;

static void video_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}
}

static void destroy_window()
{
	if (renderer){
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window){
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

static void create_window()
{
	destroy_window();

	window = SDL_CreateWindow(APPNAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
	if (window == NULL){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
}

static void create_pixels()
{
	if (texture){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (pixels){
		free(pixels);
		pixels = NULL;
	}
	pixels = (Uint32 *)malloc(width * height * BPP);
}

static void quit()
{
	destroy_window();
	SDL_Quit();
}

static Uint32 map_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 p = 0;
	p = ((r << 0) & 0xff) + ((g & 0xff) << 8) + ((b & 0xff) << 16) + ((a & 0xff) << 24);
	return p;
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
	create_pixels();

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
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event){
						case SDL_WINDOWEVENT_RESIZED:
						{
							//TODO: this doesn't really work yet
							/*
							width = event.window.data1;
							height = event.window.data2;
							create_pixels();
							*/
							//fprintf(stdout, "window resized to %i, %i\n", event.window.data1, event.window.data2);
						} break;
						default:
						{
							//fprintf(stdout, "sdl window event: %i\n", event.window.event);
						} break;
					}
				} break;
				default:
				{
					fprintf(stdout, "sdl event: %i\n", event.type);
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

		// update texture
		for (int _x=0; _x<width; _x++){
			for (int _y=0; _y<480; _y++){
				int pitch = width * 2;
				int offset = _x + (_y * pitch);
				pixels[offset] = map_rgba(((_x + x) & 0xff), ((_y + y) & 0xff), 0, 0);
			}
		}

		// blit pixels
		SDL_UpdateTexture(texture, NULL, pixels, width * BPP);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	quit();
}
