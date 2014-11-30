#include "game.h"

Sprite::Sprite(int width, int height)
{
	this->width = width;
	this->height = height;
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
	src.w = width;
	src.h = height;
	SDL_Rect dst;
	dst.x = location.x - (width / 2);
	dst.y = location.y - (height / 2);
	dst.w = width;
	dst.h = height;

	SDL_RenderCopy(renderer, texture, &src, &dst);
}

SDL_Texture *Sprite::_load_image(SDL_Renderer *renderer, const char *file)
{
	SDL_Surface *tmp = NULL;
	tmp = IMG_Load(file);
	if (tmp == NULL){
		fprintf(stderr, "failed to load sprite image\n");
		return NULL;
	}

	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);

	return t;
}

void Sprite::load_image(SDL_Renderer *renderer, const char *file)
{
	if (texture != NULL){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	texture = _load_image(renderer, file);
	if (texture == NULL){
		fprintf(stderr, "failed to create sprite texture: %s\n", SDL_GetError());
	}
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
