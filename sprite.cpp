#include "game.h"

Sprite::Sprite(int width, int height)
{
	this->width = width;
	this->height = height;
	texture = NULL;
	animated = 0;
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

void Sprite::render_animation(SDL_Renderer *renderer)
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

	SDL_RenderCopy(renderer, animation[current_animation], &src, &dst);
	Uint32 now = SDL_GetTicks();
	if (now - last_animation_tick > 50){
		current_animation++;
		if (current_animation >= animated){
			current_animation = 0;
		}
		last_animation_tick = now;
	}
}

void Sprite::render(SDL_Renderer *renderer)
{
	if (is_animated() && can_move('x')){
		if (motion.active){
			render_animation(renderer);
			return;
		}
	}
	render_static(renderer);
}
void Sprite::render_static(SDL_Renderer *renderer)
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

bool Sprite::is_animated()
{
	return (animated > 0);
}

void Sprite::enable_animation(int frames)
{
	animated = frames;
	animation = new SDL_Texture *[frames];
	animation_index = -1;
	current_animation = 0;
	last_animation_tick = SDL_GetTicks();
}

void Sprite::load_animation(SDL_Renderer *renderer, const char *file)
{
	// make sure we have room to add
	if (animation_index == animated - 1){
		return;
	}

	animation_index++;
	animation[animation_index] = _load_image(renderer, file);
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
