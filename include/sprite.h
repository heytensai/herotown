#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "point.h"
#include "video.h"

class Animation
{
private:
	Uint32 frames;
	Uint32 last_loaded_texture;

public:
	const static int NONE = 0x0000;
	const static int WALK_LEFT = 0x0001;
	const static int WALK_RIGHT = 0x0002;
	const static int TICK = 0x0003;
	const static int BOOM = 0x0004;
	const static int SPIN = 0x0005;
	SDL_Texture **texture;
	int name;
	Uint32 speed;
	int width;
	int height;

	Animation();
	~Animation();
	Uint32 get_frames();
	void set_frames(int frames);
	void load_image(Video *video, const char *file);
	SDL_Texture *get_texture(Uint32 frame);
};

class Sprite
{
private:
	Video *video;
	static const int ANIMATION_MAX = 8;
	Animation *animation[ANIMATION_MAX];
	int animation_count;
	int active_animation;
	Uint32 animation_frame;
	SDL_Texture *texture;
	bool use_animation_always;
	Uint32 last_animation_tick;

	void render_animation();
	void render_static();

protected:
	SDL_Texture *_load_image(const char *file);

public:
	velocity_t velocity;
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

	Sprite(Video *video, int width, int height);
	~Sprite();
	bool moving();
	bool can_move(char direction);
	void load_image(const char *file);
	void render();
	bool is_animated();
	bool intersects(Sprite *other, int buffer);
	bool intersects(Sprite *other, int buffer, int direction);
	bounding_box_t get_bounding_box(int buffer = 0);
	void always_animate(bool b);
	bool add_animation(Animation *animation);
	bool set_animation(const int name);

};

class Coin : public Sprite
{
private:
	static Animation animation_1;
public:
	Coin(Video *video);
};

class Block : public Sprite
{
public:
	Block(Video *video);
};

class Fireball : public Sprite
{
public:
	Fireball(Video *video);
};

#endif /* SPRITE_H */
