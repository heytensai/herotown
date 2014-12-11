#include "hero.h"

Hero::Hero(Video *video, int width, int height)
	: Sprite(video, width, height)
{
	direction = 0;
	score = 0;
}

void Hero::subtract_coins(int count)
{
	score -= count;
	if (score < 0) score = 0;
}
