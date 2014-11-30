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
	int width;
	int height;
	static const int step = 4;
	static const int edge_size = 80;

	Sprite(int width, int height);
	~Sprite();
	bool moving();
	bool can_move(char direction);
	void load_image(SDL_Renderer *renderer, const char *file);
	void render(SDL_Renderer *renderer);

};

#endif /* SPRITE_H */
