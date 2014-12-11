#include "sprite.h"

Animation::Animation()
{
	frames = -1;
	texture = NULL;
	last_loaded_texture = -1;
}

Animation::~Animation()
{
	if (texture != NULL){
		for (int i=0; i<frames; i++){
			SDL_DestroyTexture(texture[i]);
		}
		delete texture;
		texture = NULL;
	}
}

int Animation::get_frames()
{
	return frames;
}

void Animation::set_frames(int frames)
{
	this->frames = frames;
	texture = new SDL_Texture *[frames];
	memset(texture, 0, sizeof(SDL_Texture *) * frames);
}

void Animation::load_image(SDL_Renderer *renderer, const char *file)
{
	if (last_loaded_texture == frames - 1){
		return;
	}
	SDL_Surface *tmp = NULL;
	tmp = IMG_Load(file);
	if (tmp == NULL){
		fprintf(stderr, "failed to load sprite image\n");
		return;
	}

	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	if (t == NULL){
		fprintf(stderr, "failed to load sprite texture\n");
		return;
	}
	last_loaded_texture++;
	texture[last_loaded_texture] = t;
}

Sprite::Sprite(int width, int height)
{
	this->width = width;
	this->height = height;
	texture = NULL;
	hidden = false;
	use_animation_always = false;
	animation_count = -1;
	memset(animation, 0, sizeof(animation));
}

Sprite::~Sprite()
{
	if (texture != NULL){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

bool Sprite::add_animation(Animation *animation)
{
	if (animation_count == ANIMATION_MAX - 1){
		return false;
	}
	animation_count++;
	this->animation[animation_count] = animation;
	return true;
}

bool Sprite::set_animation(const int name)
{
	if (name != Animation::NONE){
		for (int i=0; i<=animation_count; i++){
			if (animation[i]->name == name){
				active_animation = i;
				if (animation_frame >= animation[i]->get_frames()){
					animation_frame = 0;
				}
				return true;
			}
		}
	}
	active_animation = -1;
	return false;
}

bounding_box_t Sprite::get_bounding_box(int buffer)
{
	bounding_box_t b;
	b.top_left.x = location.x - (width / 2) - buffer;
	b.bottom_right.x = location.x + (width / 2) + buffer;
	b.top_left.y = location.y - (height / 2) - buffer;
	b.bottom_right.y = location.y + (height / 2) + buffer;
	return b;
}

bool Sprite::intersects(Sprite *other, int buffer, int direction)
{
	if (other == NULL){
		return false;
	}

	bounding_box_t me = get_bounding_box(buffer);
	bounding_box_t them = other->get_bounding_box(buffer);

	switch (direction){
		case DIRECTION_UP:
		{
			if (me.bottom_right.x <= them.top_left.x) return false;
			if (me.top_left.x >= them.bottom_right.x) return false;
			if (me.bottom_right.y < them.bottom_right.y) return false;
			if (me.top_left.y > them.bottom_right.y) return false;
			return true;
		};
		case DIRECTION_DOWN:
		{
			/*
			printf("me (%d, %d)              : them (%d, %d)\n"
			       "                (%d, %d) :                 (%d, %d)\n",
				me.top_left.x, me.top_left.y,
				me.bottom_right.x, me.bottom_right.y,
				them.top_left.x, them.top_left.y,
				them.bottom_right.x, them.bottom_right.y
			);
			*/
			if (me.bottom_right.x <= them.top_left.x) return false;
			if (me.top_left.x >= them.bottom_right.x) return false;
			if (me.top_left.y > them.top_left.y) return false;
			if (me.bottom_right.y < them.top_left.y) return false;
			return true;
		};
		case DIRECTION_LEFT:
		{
			if (me.top_left.y >= them.bottom_right.y) return false;
			if (me.bottom_right.y <= them.top_left.y) return false;
			if (me.top_left.x > them.bottom_right.x) return false;
			if (me.bottom_right.x < them.bottom_right.x) return false;
			return true;
		};
		case DIRECTION_RIGHT:
		{
			if (me.top_left.y >= them.bottom_right.y) return false;
			if (me.bottom_right.y <= them.top_left.y) return false;
			if (me.top_left.x > them.top_left.x) return false;
			if (me.bottom_right.x < them.top_left.x) return false;
			return true;
		};
	}

	return true;
}

bool Sprite::intersects(Sprite *other, int buffer)
{
	if (other == NULL){
		return false;
	}

	bounding_box_t me = get_bounding_box(buffer);
	bounding_box_t them = other->get_bounding_box(buffer);

	if (me.top_left.x >= them.bottom_right.x){
		return false;
	}

	if (me.bottom_right.x <= them.top_left.x){
		return false;
	}

	if (me.bottom_right.y <= them.top_left.y){
		return false;
	}

	if (me.top_left.y >= them.bottom_right.y){
		return false;
	}

	return true;
}

bool Sprite::moving()
{
	return (motion.movement.x != 0) || (motion.movement.y != 0);
}

void Sprite::render_animation(SDL_Renderer *renderer)
{
	Animation *a = animation[active_animation];
	SDL_Texture *tex = a->texture[animation_frame];

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = a->width;
	src.h = a->height;
	SDL_Rect dst;
	dst.x = location.x - (a->width / 2);
	dst.y = location.y - (a->height / 2);
	dst.w = a->width;
	dst.h = a->height;

	SDL_RenderCopy(renderer, tex, &src, &dst);
	Uint32 now = SDL_GetTicks();
	if (now - last_animation_tick > a->speed){
		animation_frame++;
		if (animation_frame >= a->get_frames()){
			animation_frame = 0;
		}
		last_animation_tick = now;
	}
}

void Sprite::render(SDL_Renderer *renderer)
{
	if (hidden){
		return;
	}

	if (is_animated() && (use_animation_always || moving())){
		render_animation(renderer);
	}
	else{
		render_static(renderer);
	}
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
	return (animation_count > -1);
}

void Sprite::always_animate(bool b)
{
	use_animation_always = b;
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
