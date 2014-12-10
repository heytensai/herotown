#include "hero.h"

Hero::Hero(int width, int height)
	: Sprite(width, height)
{
	direction = 0;
	score = 0;
}

void Hero::subtract_coins(int count)
{
	score -= count;
	if (score < 0) score = 0;
}
