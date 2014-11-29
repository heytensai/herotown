#include "SDL2/SDL.h"
#include "game.h"
#include "sprite.h"

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
static Sprite sprite;

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

void draw_sprite()
{
	// move sprite (if sprite is in bounds)
	if (sprite.motion.active){
		if (sprite.can_move('x')){
			sprite.location.x += sprite.motion.movement.x;
		}

		if (sprite.can_move('y')){
			sprite.location.y += sprite.motion.movement.y;
		}
	}

	// draw sprite
	int offset;
	for (int x=-Sprite::size; x<Sprite::size; x++){
		for (int y=-Sprite::size; y<Sprite::size; y++){
			offset = pitch_offset(sprite.location.x + x, sprite.location.y + y);
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
						sprite.motion.active = 1;
						sprite.motion.movement.y = -Sprite::step;
					} break;
					case SDLK_s:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.y = Sprite::step;
					} break;
					case SDLK_a:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.x = -Sprite::step;
					} break;
					case SDLK_d:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.x = Sprite::step;
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
						sprite.motion.active = 1;
						sprite.motion.movement.y = 0;
						if (!sprite.motion.movement.x && !sprite.motion.movement.y){
							sprite.motion.active = 0;
						}
					} break;
					case SDLK_a:
					case SDLK_d:
					{
						sprite.motion.movement.x = 0;
						if (!sprite.motion.movement.x && !sprite.motion.movement.y){
							sprite.motion.active = 0;
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

	sprite.location.x = 100;
	sprite.location.y = 100;

	sprite.motion.active = 0;
	sprite.motion.movement.x = 0;
	sprite.motion.movement.y = 0;
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
