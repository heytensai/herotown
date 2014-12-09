#ifndef BOMB_H
#define BOMB_H

#include "globals.h"
#include "sprite.h"

class Bomb : public Sprite
{
private:
	Uint32 start;

public:
	static const int TIME_LIMIT = 3000;
	static const int RANGE = 10;

	Bomb(SDL_Renderer *renderer);
	bool ready_to_explode();

};

#endif /* BOMB_H */
