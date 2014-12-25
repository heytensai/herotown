#ifndef BOMB_H
#define BOMB_H

#include "globals.h"
#include "sprite.h"

class Bomb : public Sprite
{
private:
	Uint32 start;
	int explode_start;
	static Animation animation_1;
	static Animation animation_2;

public:
	static const int TIME_LIMIT = 3000;
	static const int EXPLODE_TIME = 20 * 5;
	static const int RANGE = 30;

	Bomb(Video *video);
	bool ready_to_explode();
	void explode();
	bool exploded();
	bool exploding();

};

#endif /* BOMB_H */
