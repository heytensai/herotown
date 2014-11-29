#include "game.h"

Video::Video(int width, int height)
{
	set_dimensions(width, height);
}

Uint32 Video::pitch_offset(int x, int y)
{
	// why does pitch == width * 2?
	return (x + (y * width * 2));
}

void Video::set_pixel(int x, int y, int r, int g, int b)
{
	if (pixels == NULL){
		return;
	}
	int offset = pitch_offset(x, y);
	Uint32 p = map_rgba(r, g, b, 0);
	pixels[offset] = p;
}

void Video::set_dimensions(int width, int height)
{
	this->width = width;
	this->height = height;

	//TODO: resize
}

void Video::destroy_window()
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

Uint32 Video::map_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 p = 0;
	p = ((r << 0) & 0xff) + ((g & 0xff) << 8) + ((b & 0xff) << 16) + ((a & 0xff) << 24);
	return p;
}

void Video::create_window()
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

void Video::create_pixels()
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

void Video::quit()
{
	destroy_window();
	SDL_Quit();
}

void Video::blit()
{
	SDL_UpdateTexture(texture, NULL, pixels, width * BPP);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Video::init()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}
}

