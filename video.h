#ifndef VIDEO_H
#define VIDEO_H

#include "SDL2/SDL.h"

class Video
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

#endif /* VIDEO_H */
