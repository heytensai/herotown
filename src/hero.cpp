#include "hero.h"

Hero::Hero(Video *video, int width, int height)
	: Sprite(video, width, height)
{
	direction = 0;
	score = 0;
	motion.active = 0;
	motion.movement.x = 0;
	motion.movement.y = 0;
	last_bomb_added = 0;
}

void Hero::subtract_coins(int count)
{
	score -= count;
	if (score < 0) score = 0;
}

Mario::Mario(Video *video)
	: Hero(video, 48, 64)
{
	direction = 0;
	score = 0;
	load_image("resources/mario0.png");

	Animation *a = new Animation();
	a->set_frames(8);
	a->name = Animation::WALK_RIGHT;
	a->speed = 40;
	a->width = width;
	a->height = height;
	a->load_image(video, "resources/mario0.png");
	a->load_image(video, "resources/mario1.png");
	a->load_image(video, "resources/mario2.png");
	a->load_image(video, "resources/mario3.png");
	a->load_image(video, "resources/mario4.png");
	a->load_image(video, "resources/mario5.png");
	a->load_image(video, "resources/mario6.png");
	a->load_image(video, "resources/mario7.png");
	a->load_image(video, "resources/mario8.png");
	add_animation(a);

	a = new Animation();
	a->set_frames(8);
	a->name = Animation::WALK_LEFT;
	a->speed = 40;
	a->width = width;
	a->height = height;
	a->load_image(video, "resources/mario-left0.png");
	a->load_image(video, "resources/mario-left1.png");
	a->load_image(video, "resources/mario-left2.png");
	a->load_image(video, "resources/mario-left3.png");
	a->load_image(video, "resources/mario-left4.png");
	a->load_image(video, "resources/mario-left5.png");
	a->load_image(video, "resources/mario-left6.png");
	a->load_image(video, "resources/mario-left7.png");
	a->load_image(video, "resources/mario-left8.png");
	add_animation(a);

	set_animation(Animation::NONE);
}

Yoshi::Yoshi(Video *video)
	: Hero(video, 48, 64)
{
	direction = 0;
	score = 0;
	load_image("resources/yoshi0.png");

	Animation *a;
	a = new Animation();
	a->set_frames(8);
	a->name = Animation::WALK_RIGHT;
	a->speed = 50;
	a->width = width;
	a->height = height;
	a->load_image(video, "resources/yoshi0.png");
	a->load_image(video, "resources/yoshi1.png");
	a->load_image(video, "resources/yoshi2.png");
	a->load_image(video, "resources/yoshi3.png");
	a->load_image(video, "resources/yoshi4.png");
	a->load_image(video, "resources/yoshi5.png");
	a->load_image(video, "resources/yoshi6.png");
	a->load_image(video, "resources/yoshi7.png");
	a->load_image(video, "resources/yoshi8.png");
	add_animation(a);

	a = new Animation();
	a->set_frames(8);
	a->name = Animation::WALK_LEFT;
	a->speed = 50;
	a->width = width;
	a->height = height;
	a->load_image(video, "resources/yoshi-left0.png");
	a->load_image(video, "resources/yoshi-left1.png");
	a->load_image(video, "resources/yoshi-left2.png");
	a->load_image(video, "resources/yoshi-left3.png");
	a->load_image(video, "resources/yoshi-left4.png");
	a->load_image(video, "resources/yoshi-left5.png");
	a->load_image(video, "resources/yoshi-left6.png");
	a->load_image(video, "resources/yoshi-left7.png");
	a->load_image(video, "resources/yoshi-left8.png");
	add_animation(a);

	set_animation(Animation::NONE);
}

