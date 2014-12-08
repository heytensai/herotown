#include "hero.h"

Hero::Hero(int width, int height)
	: Sprite(width, height)
{
	direction = 0;
	score = 0;
}
