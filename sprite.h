#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"

class Sprite
{
public:
	point_t location;
	movement_t motion;
	static const int size = 5;
	static const int step = 1;

	bool moving();

};

#endif /* SPRITE_H */
