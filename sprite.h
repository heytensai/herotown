#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "point.h"

class Animation
{
private:
	int frames;
	int last_loaded_texture;

public:
	const static int NONE = 0x0000;
	const static int WALK_LEFT = 0x0001;
	const static int WALK_RIGHT = 0x0002;
	const static int TICK = 0x0003;
	SDL_Texture **texture;
	int name;
	Uint32 speed;
	int width;
	int height;

	Animation();
	~Animation();
	int get_frames();
	void set_frames(int frames);
	void load_image(SDL_Renderer *renderer, const char *file);
};

class Sprite
{
private:
	static const int ANIMATION_MAX = 8;
	Animation *animation[ANIMATION_MAX];
	int animation_count;
	int active_animation;
	int animation_frame;
	SDL_Texture *texture;
	bool use_animation_always;
	Uint32 last_animation_tick;

	void render_animation(SDL_Renderer *renderer);
	void render_static(SDL_Renderer *renderer);

protected:
	SDL_Texture *_load_image(SDL_Renderer *renderer, const char *file);

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
	void render(SDL_Renderer *renderer);
	bool is_animated();
	bool intersects(Sprite *other, int buffer);
	bool intersects(Sprite *other, int buffer, int direction);
	bounding_box_t get_bounding_box(int buffer = 0);
	void always_animate(bool b);
	bool add_animation(Animation *animation);
	bool set_animation(const int name);

};

#endif /* SPRITE_H */
