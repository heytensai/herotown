#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "point.h"

class Sprite
{
private:
	SDL_Texture **animation;
	SDL_Texture *texture;
	int animated;
	int animation_index;
	int current_animation;
	Uint32 last_animation_tick;

	SDL_Texture *_load_image(SDL_Renderer *renderer, const char *file);
	void render_animation(SDL_Renderer *renderer);
	void render_static(SDL_Renderer *renderer);

public:
	point_t location;
	movement_t motion;
	int width;
	int height;
	bool hidden;
	static const int step = 4;
	static const int edge_size = 80;

	Sprite(int width, int height);
	~Sprite();
	bool moving();
	bool can_move(char direction);
	void load_image(SDL_Renderer *renderer, const char *file);
	void load_animation(SDL_Renderer *renderer, const char *file);
	void render(SDL_Renderer *renderer);
	void enable_animation(int frames);
	bool is_animated();
	bool intersects(Sprite *other);
	bounding_box_t get_bounding_box();

};

#endif /* SPRITE_H */
