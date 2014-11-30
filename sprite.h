#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"

class Sprite
{
private:

public:
	point_t location;
	movement_t motion;
	SDL_Texture *texture;
	static const int size = 5;
	static const int step = 4;
	static const int edge_size = 80;

	Sprite();
	~Sprite();
	bool moving();
	bool can_move(char direction);
	void load_image(SDL_Renderer *renderer, const char *file);
	void render(SDL_Renderer *renderer);

};

#endif /* SPRITE_H */
