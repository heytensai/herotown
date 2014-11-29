#ifndef SDL_H
#define SDL_H

#include "SDL2/SDL.h"
#include "game.h"

class SDL
{
public:
	SDL_Window *window;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	Uint32 *pixels = NULL;

	void init();
	void blit();
	void destroy_window();
	void create_window();
	void create_pixels();
	void quit();
	Uint32 map_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

};

#endif /* SDL_H */
