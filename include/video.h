#ifndef VIDEO_H
#define VIDEO_H

#include "globals.h"

class Video
{
private:
	Uint32 pitch_offset(int x, int y);
	SDL_Texture *background = NULL;
	SDL_Window *window;
	TTF_Font *font;

	void init_font();

public:
	int width;
	int height;
	SDL_Renderer *renderer = NULL;

	Video(int width, int height);
	~Video();
	void init();
	void blit_background();
	void set_pixel(int x, int y, int r, int g, int b);
	void set_dimensions(int width, int height);
	void destroy_window();
	void create_window();
	void load_background(const char *file);
	SDL_Texture *make_texture_from_image(const char *file);
	void quit();
	Uint32 map_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void start_render();
	void finish_render();
	void render_text(int x, int y, const char *text);

};

#endif /* VIDEO_H */
