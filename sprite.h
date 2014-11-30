#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"

class Sprite
{
public:
	point_t location;
	movement_t motion;
	static const int size = 5;
	static const int step = 2;
	static const int edge_size = 80;

	bool moving();
	bool can_move(char direction);

};

#endif /* SPRITE_H */
