#include "game.h"

Sprite::Sprite()
{
	texture = NULL;
}

Sprite::~Sprite()
{
	if (texture != NULL){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

bool Sprite::moving()
{
	return (motion.movement.x != 0) || (motion.movement.y != 0);
}

void Sprite::render(SDL_Renderer *renderer)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;
	SDL_Rect dst;
	dst.x = location.x - 32;
	dst.y = location.y - 32;
	dst.w = 64;
	dst.h = 64;

	SDL_RenderCopy(renderer, texture, &src, &dst);
}

void Sprite::load_image(SDL_Renderer *renderer, const char *file)
{
	SDL_Surface *tmp = NULL;
	tmp = IMG_Load(file);
	if (tmp == NULL){
		fprintf(stderr, "failed to load sprite image\n");
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, tmp);
	if (texture == NULL){
		fprintf(stderr, "failed to create sprite texture: %s\n", SDL_GetError());
	}
	SDL_FreeSurface(tmp);
}

bool Sprite::can_move(char direction)
{
	switch (direction){
		case 'x':
		{
			if ((location.x <= (0 + Sprite::edge_size)) && (motion.movement.x < 0)){
				return false;
			}
			if ((location.x >= (WIDTH - Sprite::edge_size)) && (motion.movement.x > 0)){
				return false;
			}
		} break;
		case 'y':
		{
			if ((location.y <= (0 + Sprite::edge_size)) && (motion.movement.y < 0)){
				return false;
			}
			if ((location.y >= (HEIGHT - Sprite::edge_size)) && (motion.movement.y > 0)){
				return false;
			}
		} break;
	}
	return true;
}
