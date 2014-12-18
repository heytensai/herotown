#include "bomb.h"

Animation Bomb::animation_1;
Animation Bomb::animation_2;

Bomb::Bomb(Video *video)
	: Sprite(video, 46, 57)
{
	start = SDL_GetTicks();
	motion.active = 0;
	motion.movement.x = 0;
	motion.movement.y = 0;
	load_image("resources/bomb.png");

	if (animation_1.get_frames() != 2){
		animation_1.set_frames(2);
		animation_1.name = Animation::TICK;
		animation_1.speed = 200;
		animation_1.width = width;
		animation_1.height = height;
		animation_1.load_image(video, "resources/bomb0.png");
		animation_1.load_image(video, "resources/bomb1.png");
	}
	add_animation(&animation_1);
	set_animation(Animation::TICK);

	if (animation_2.get_frames() != 5){
		animation_2.set_frames(5);
		animation_2.name = Animation::BOOM;
		animation_2.speed = 20;
		animation_2.width = 103;
		animation_2.height = 101;
		animation_2.load_image(video, "resources/bomb-explosion0.png");
		animation_2.load_image(video, "resources/bomb-explosion1.png");
		animation_2.load_image(video, "resources/bomb-explosion2.png");
		animation_2.load_image(video, "resources/bomb-explosion3.png");
		animation_2.load_image(video, "resources/bomb-explosion4.png");
	}
	add_animation(&animation_2);

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
