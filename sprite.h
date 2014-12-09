#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "point.h"


class Sprite
{
private:
	SDL_Texture **animation_right;
	SDL_Texture **animation_left;
	SDL_Texture *texture;
	int animated_left;
	int animated_right;
	int animation_index_left;
	int animation_index_right;
	int current_animation_left;
	int current_animation_right;
	Uint32 last_animation_tick;

	SDL_Texture *_load_image(SDL_Renderer *renderer, const char *file);
	void render_animation_left(SDL_Renderer *renderer);
	void render_animation_right(SDL_Renderer *renderer);
	void render_animation(SDL_Renderer *renderer);
	void render_static(SDL_Renderer *renderer);

public:
	point_t location;
	movement_t motion;
	int width;
	int height;
	bool hidden;
	static const int step = 4;
	static const int edge_size = 00;
	static const int DIRECTION_UP = 0x01;
	static const int DIRECTION_DOWN = 0x02;
	static const int DIRECTION_LEFT = 0x04;
	static const int DIRECTION_RIGHT = 0x08;

	Sprite(int width, int height);
	~Sprite();
	bool moving();
	bool can_move(char direction);
	void load_image(SDL_Renderer *renderer, const char *file);
	void load_animation_left(SDL_Renderer *renderer, const char *file);
	void load_animation_right(SDL_Renderer *renderer, const char *file);
	void render(SDL_Renderer *renderer);
	void enable_animation_left(int frames);
	void enable_animation_right(int frames);
	bool is_animated();
	bool intersects(Sprite *other, int buffer);
	bool intersects(Sprite *other, int buffer, int direction);
	bounding_box_t get_bounding_box(int buffer = 0);

};

#endif /* SPRITE_H */
