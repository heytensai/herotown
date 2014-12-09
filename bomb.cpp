#include "bomb.h"

Bomb::Bomb(SDL_Renderer *renderer)
	: Sprite(16, 16)
{
	start = SDL_GetTicks();
	motion.active = 0;
	motion.movement.x = 0;
	motion.movement.y = 0;
	load_image(renderer, "resources/bomb.png");
	explode_start = 0;
}

void Bomb::explode()
{
	explode_start = SDL_GetTicks();
}

bool Bomb::exploded()
{
	if (!explode_start) return false;

	Uint32 now = SDL_GetTicks();
	Uint32 life = now - explode_start;
	if (life >= EXPLODE_TIME){
		printf("now=%i, life=%i, explode_start=%i\n", now, life, explode_start);
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
