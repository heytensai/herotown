#include "bomb.h"

Bomb::Bomb(SDL_Renderer *renderer)
	: Sprite(46, 57)
{
	start = SDL_GetTicks();
	motion.active = 0;
	motion.movement.x = 0;
	motion.movement.y = 0;
	load_image(renderer, "resources/bomb.png");

	Animation *a = new Animation();
	a->set_frames(2);
	a->name = Animation::TICK;
	a->speed = 200;
	a->width = width;
	a->height = height;
	a->load_image(renderer, "resources/bomb0.png");
	a->load_image(renderer, "resources/bomb1.png");
	add_animation(a);
	set_animation(Animation::TICK);

	a = new Animation();
	a->set_frames(5);
	a->name = Animation::BOOM;
	a->speed = 20;
	a->width = 103;
	a->height = 101;
	a->load_image(renderer, "resources/bomb-explosion0.png");
	a->load_image(renderer, "resources/bomb-explosion1.png");
	a->load_image(renderer, "resources/bomb-explosion2.png");
	a->load_image(renderer, "resources/bomb-explosion3.png");
	a->load_image(renderer, "resources/bomb-explosion4.png");
	add_animation(a);

	always_animate(true);
	explode_start = 0;
}

void Bomb::explode()
{
	explode_start = SDL_GetTicks();
	set_animation(Animation::BOOM);
}

bool Bomb::exploded()
{
	if (!explode_start) return false;

	Uint32 now = SDL_GetTicks();
	Uint32 life = now - explode_start;
	if (life >= EXPLODE_TIME){
		return true;
	}
	return false;
}

bool Bomb::ready_to_explode()
{
	if (explode_start) return false;

	Uint32 now = SDL_GetTicks();
	Uint32 life = now - start;
	if (life >= TIME_LIMIT){
		return true;
	}
	return false;
}
