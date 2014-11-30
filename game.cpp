#include "game.h"

Game::Game(int width, int height)
{
	this->width = width;
	this->height = height;
	running = 1;
	video = new Video(width, height);

	background.active = 0;
	background.movement.x = 0;
	background.movement.y = 0;

	sprite.location.x = 100;
	sprite.location.y = 100;

	sprite.motion.active = 0;
	sprite.motion.movement.x = 0;
	sprite.motion.movement.y = 0;

	video->init();
	video->create_window();
	video->create_pixels();

	sound.init();
}

void Game::draw_sprite()
{
	// move sprite (if sprite is in bounds)
	if (sprite.motion.active){
		if (sprite.can_move('x')){
			sprite.location.x += sprite.motion.movement.x;
		}

		if (sprite.can_move('y')){
			sprite.location.y += sprite.motion.movement.y;
		}
	}

	// draw sprite
	int offset;
	for (int x=-Sprite::size; x<Sprite::size; x++){
		for (int y=-Sprite::size; y<Sprite::size; y++){
			video->set_pixel(
				sprite.location.x + x,
				sprite.location.y + y,
				0xff,
				0xff,
				0xff
			);
		}
	}
}

void Game::draw_background()
{
	if (sprite.moving()){
		if (!sprite.can_move('x')){
			if (sprite.motion.movement.x < 0){
				sprite.location.x -= sprite.motion.movement.x;
				background.movement.x = -Sprite::step;
				background.active = 1;
			}
			else if (sprite.motion.movement.x > 0){
				sprite.location.x -= sprite.motion.movement.x;
				background.movement.x = Sprite::step;
				background.active = 1;
			}
		}

		if (!sprite.can_move('y')){
			if (sprite.motion.movement.y < 0){
				sprite.location.y -= sprite.motion.movement.y;
				background.movement.y = -Sprite::step;
				background.active = 1;
			}
			else if (sprite.motion.movement.y > 0){
				sprite.location.y -= sprite.motion.movement.y;
				background.movement.y = Sprite::step;
				background.active = 1;
			}
		}
	}
	else{
		background.movement.x = 0;
		background.movement.y = 0;
		background.active = 0;
	}

	// move grid
	if (background.active){
		grid_base.y += background.movement.y;
		grid_base.x += background.movement.x;
	}

	// update texture
	for (int x=0; x<width; x++){
		for (int y=0; y<height; y++){
			video->set_pixel(
				x, y,
				((x + grid_base.x) & 0xff),
				((y + grid_base.y) & 0xff),
				((y + grid_base.y + x + grid_base.x) & 0xff)
			);
		}
	}
}

void Game::process_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
			{
				running = 0;
			} break;
			case SDL_KEYDOWN:
			{
				if (event.key.repeat){
					break;
				}
				switch (event.key.keysym.sym){
					case SDLK_b:
					{
						sound.play(Sound::SOUND_BEEP);
					} break;
					case SDLK_w:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.y = -Sprite::step;
					} break;
					case SDLK_s:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.y = Sprite::step;
					} break;
					case SDLK_a:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.x = -Sprite::step;
					} break;
					case SDLK_d:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.x = Sprite::step;
					} break;
					case SDLK_LEFT:
					{
						background.active = 1;
						background.movement.x = 1;
					} break;
					case SDLK_RIGHT:
					{
						background.active = 1;
						background.movement.x = -1;
					} break;
					case SDLK_UP:
					{
						background.active = 1;
						background.movement.y = -1;
					} break;
					case SDLK_DOWN:
					{
						background.active = 1;
						background.movement.y = 1;
					} break;
					case SDLK_SPACE:
					{
						background.active = !background.active;
					} break;
					case SDLK_q:
					case SDLK_ESCAPE:
					{
						running = 0;
					} break;
				}
			} break;
			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym){
					case SDLK_w:
					case SDLK_s:
					{
						sprite.motion.active = 1;
						sprite.motion.movement.y = 0;
						if (!sprite.motion.movement.x && !sprite.motion.movement.y){
							sprite.motion.active = 0;
						}
					} break;
					case SDLK_a:
					case SDLK_d:
					{
						sprite.motion.movement.x = 0;
						if (!sprite.motion.movement.x && !sprite.motion.movement.y){
							sprite.motion.active = 0;
						}
					} break;
					case SDLK_RIGHT:
					case SDLK_LEFT:
					{
						background.active = 0;
						background.movement.x = 0;
					} break;
					case SDLK_UP:
					case SDLK_DOWN:
					{
						background.active = 0;
						background.movement.y = 0;
					} break;
				}
			} break;
			case SDL_TEXTINPUT:
			{
				//ignore
			} break;
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event){
					case SDL_WINDOWEVENT_RESIZED:
					{
						//TODO: this doesn't really work yet
						/*
						width = event.window.data1;
						height = event.window.data2;
						create_pixels();
						*/
						//fprintf(stdout, "window resized to %i, %i\n", event.window.data1, event.window.data2);
					} break;
					default:
					{
						//fprintf(stdout, "sdl window event: %i\n", event.window.event);
					} break;
				}
			} break;
			default:
			{
				fprintf(stdout, "sdl event: %x\n", event.type);
			} break;
		}
	}
}

