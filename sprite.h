#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"

class Sprite
{
private:
	SDL_Texture *_load_image(SDL_Renderer *renderer, const char *file);
	SDL_Texture **animation;
	int animated;
	int animation_index;
	int current_animation;
	Uint32 last_animation_tick;
	void render_animation(SDL_Renderer *renderer);
	void render_static(SDL_Renderer *renderer);

public:
	point_t location;
	movement_t motion;
	SDL_Texture *texture;
	int width;
	int height;
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

};

#endif /* SPRITE_H */
