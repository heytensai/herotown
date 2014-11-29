#include "SDL2/SDL.h"
#include "game.h"

#define APPNAME "sdlapp"
#define WIDTH 800
#define HEIGHT 600
#define COLORDEPTH 32
#define BPP (COLORDEPTH / 4)
#define SPRITE_SIZE 5
#define SPRITE_STEP 1

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static Uint32 *pixels = NULL;
static int global_running = 1;
static point_t grid_base;
static int width = WIDTH;
static int height = HEIGHT;
static int pitch = width * 2;
static movement_t vertical;
static movement_t horizontal;
static point_t sprite;
static movement_t sprite_motion;

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

static inline int pitch_offset(int x, int y)
{
	return (x + (y * pitch));
}

void draw_background()
{
	// move grid
	if (vertical.active){
		grid_base.y += vertical.movement.y;
	}
	if (horizontal.active){
		grid_base.x += horizontal.movement.x;
	}

	// update texture
	for (int x=0; x<width; x++){
		for (int y=0; y<height; y++){
			int offset = pitch_offset(x, y);
			pixels[offset] = map_rgba(((x + grid_base.x) & 0xff), ((y + grid_base.y) & 0xff), ((y + grid_base.y + x + grid_base.x) & 0xff), 0);
		}
	}
}

bool sprite_can_move(char direction)
{
	switch (direction){
		case 'x':
		{
			if ((sprite.x <= (0 + SPRITE_SIZE)) && (sprite_motion.movement.x < 0)){
				return false;
			}
			if ((sprite.x >= (WIDTH - SPRITE_SIZE)) && (sprite_motion.movement.x > 0)){
				return false;
			}
		} break;
		case 'y':
		{
			if ((sprite.y <= (0 + SPRITE_SIZE)) && (sprite_motion.movement.y < 0)){
				return false;
			}
			if ((sprite.y >= (HEIGHT - SPRITE_SIZE)) && (sprite_motion.movement.y > 0)){
				return false;
			}
		} break;
	}
	return true;
}

void draw_sprite()
{
	// move sprite (if sprite is in bounds)
	if (sprite_motion.active){
		if (sprite_can_move('x')){
			sprite.x += sprite_motion.movement.x;
		}

		if (sprite_can_move('y')){
			sprite.y += sprite_motion.movement.y;
		}
	}

	// draw sprite
	int offset;
	for (int x=-SPRITE_SIZE; x<SPRITE_SIZE; x++){
		for (int y=-SPRITE_SIZE; y<SPRITE_SIZE; y++){
			offset = pitch_offset(sprite.x + x, sprite.y + y);
			pixels[offset] = map_rgba(0xff, 0xff, 0xff, 0);
		}
	}
}

void process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				global_running = 0;
			} break;
			case SDL_KEYDOWN:
			{
				if (event.key.repeat){
					break;
				}
				switch (event.key.keysym.sym){
					case SDLK_w:
					{
						sprite_motion.active = 1;
						sprite_motion.movement.y = -SPRITE_STEP;
					} break;
					case SDLK_s:
					{
						sprite_motion.active = 1;
						sprite_motion.movement.y = SPRITE_STEP;
					} break;
					case SDLK_a:
					{
						sprite_motion.active = 1;
						sprite_motion.movement.x = -SPRITE_STEP;
					} break;
					case SDLK_d:
					{
						sprite_motion.active = 1;
						sprite_motion.movement.x = SPRITE_STEP;
					} break;
					case SDLK_LEFT:
					{
						horizontal.movement.x++;
						//fprintf(stdout, "horizontal step: %i\n", horizontal.movement.x);
					} break;
					case SDLK_RIGHT:
					{
						horizontal.movement.x--;
						//fprintf(stdout, "horizontal step: %i\n", horizontal.movement.x);
					} break;
					case SDLK_UP:
					{
						vertical.active = 1;
						vertical.movement.y = -1;
						//fprintf(stdout, "vertical step: %i\n", vertical.movement.y);
					} break;
					case SDLK_DOWN:
					{
						vertical.active = 1;
						vertical.movement.y = 1;
						//fprintf(stdout, "vertical step: %i\n", vertical.movement.y);
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
					case SDLK_w:
					case SDLK_s:
					{
						sprite_motion.active = 1;
						sprite_motion.movement.y = 0;
						if (!sprite_motion.movement.x && !sprite_motion.movement.y){
							sprite_motion.active = 0;
						}
					} break;
					case SDLK_a:
					case SDLK_d:
					{
						sprite_motion.movement.x = 0;
						if (!sprite_motion.movement.x && !sprite_motion.movement.y){
							sprite_motion.active = 0;
						}
					} break;
					case SDLK_UP:
					case SDLK_DOWN:
					{
						vertical.active = 0;
					} break;
				}
			} break;
			case SDL_TEXTINPUT:
			{
				//ignore
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
				fprintf(stdout, "sdl event: %x\n", event.type);
			} break;
		}
	}
}

void graphics_init()
{
	vertical.active = 0;
	vertical.movement.x = 0;
	vertical.movement.y = 0;

	horizontal.active = 1;
	horizontal.movement.x = 1;
	horizontal.movement.y = 0;

	sprite.x = 100;
	sprite.y = 100;

	sprite_motion.active = 0;
	sprite_motion.movement.x = 0;
	sprite_motion.movement.y = 0;
}

int main(int argc, char **argv)
{
	video_init();
	create_window();
	create_pixels();

	graphics_init();

	while (global_running){

		// process events
		process_events();

		draw_background();

		draw_sprite();

		// blit pixels
		SDL_UpdateTexture(texture, NULL, pixels, width * BPP);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	quit();
}
