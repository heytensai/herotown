#ifndef VIDEO_H
#define VIDEO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "sprite.h"

class Video
{
private:
	int width;
	int height;
	Uint32 pitch_offset(int x, int y);

public:
	SDL_Window *window;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	Uint32 *pixels = NULL;

	Video(int width, int height);
	void init();
	void blit(Sprite *s);
	void set_pixel(int x, int y, int r, int g, int b);
	void set_dimensions(int width, int height);
	void destroy_window();
	void create_window();
	void create_pixels();
	void quit();
	Uint32 map_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void start_render();
	void finish_render();

};

#endif /* VIDEO_H */
