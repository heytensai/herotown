#include "game.h"

Game::Game()
{
	running = 1;
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
						horizontal.movement.x++;
					} break;
					case SDLK_RIGHT:
					{
						horizontal.movement.x--;
					} break;
					case SDLK_UP:
					{
						vertical.active = 1;
						vertical.movement.y = -1;
					} break;
					case SDLK_DOWN:
					{
						vertical.active = 1;
						vertical.movement.y = 1;
					} break;
					case SDLK_SPACE:
					{
						horizontal.active = !horizontal.active;
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
					case SDLK_UP:
					case SDLK_DOWN:
					{
						vertical.active = 0;
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

