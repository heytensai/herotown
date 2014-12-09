#include "bomb.h"

Bomb::Bomb(SDL_Renderer *renderer)
	: Sprite(16, 16)
{
	start = SDL_GetTicks();
	motion.active = 0;
	motion.movement.x = 0;
	motion.movement.y = 0;
	load_image(renderer, "resources/bomb.png");
}

bool Bomb::ready_to_explode()
{
	Uint32 now = SDL_GetTicks();
	Uint32 life = now - start;
	if (life >= TIME_LIMIT){
		return true;
	}
	return false;
}
