#include "game.h"

Game::Game(int width, int height)
{
	this->width = width;
	this->height = height;
	running = 1;

	video = new Video(width, height);
	video->init();
	video->create_window();
	video->load_background("resources/background.png");

	background.active = 0;
	background.movement.x = 0;
	background.movement.y = 0;

	hero.location.x = 100;
	hero.location.y = 100;

	hero.motion.active = 0;
	hero.motion.movement.x = 0;
	hero.motion.movement.y = 0;

	// this must happen after video is created
	hero.load_image(video->renderer, "resources/yoshi.png");

	sound.init();
}

void Game::render()
{
	video->start_render();
	video->blit_background();
	hero.render(video->renderer);
	video->finish_render();
}

void Game::move_hero()
{
	// move hero (if hero is in bounds)
	if (hero.motion.active){
		if (hero.can_move('x')){
			hero.location.x += hero.motion.movement.x;
		}

		if (hero.can_move('y')){
			hero.location.y += hero.motion.movement.y;
		}
	}
}

void Game::move_background()
{
	if (hero.moving()){
		if (!hero.can_move('x')){
			if (hero.motion.movement.x < 0){
				hero.location.x -= hero.motion.movement.x;
				background.movement.x = -Sprite::step;
				background.active = 1;
			}
			else if (hero.motion.movement.x > 0){
				hero.location.x -= hero.motion.movement.x;
				background.movement.x = Sprite::step;
				background.active = 1;
			}
		}

		if (!hero.can_move('y')){
			if (hero.motion.movement.y < 0){
				hero.location.y -= hero.motion.movement.y;
				background.movement.y = -Sprite::step;
				background.active = 1;
			}
			else if (hero.motion.movement.y > 0){
				hero.location.y -= hero.motion.movement.y;
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
						hero.motion.active = 1;
						hero.motion.movement.y = -Sprite::step;
					} break;
					case SDLK_s:
					{
						hero.motion.active = 1;
						hero.motion.movement.y = Sprite::step;
					} break;
					case SDLK_a:
					{
						hero.motion.active = 1;
						hero.motion.movement.x = -Sprite::step;
					} break;
					case SDLK_d:
					{
						hero.motion.active = 1;
						hero.motion.movement.x = Sprite::step;
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
						hero.motion.active = 1;
						hero.motion.movement.y = 0;
						if (!hero.motion.movement.x && !hero.motion.movement.y){
							hero.motion.active = 0;
						}
					} break;
					case SDLK_a:
					case SDLK_d:
					{
						hero.motion.movement.x = 0;
						if (!hero.motion.movement.x && !hero.motion.movement.y){
							hero.motion.active = 0;
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

