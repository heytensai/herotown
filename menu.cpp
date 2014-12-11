#include "menu.h"

IntroMenu::IntroMenu(Video *video)
	: Menu(video)
{
	timer_option = 60;
}

bool IntroMenu::exit()
{
	return (result > 0);
}

void IntroMenu::event_loop()
{
	while (running){
		SDL_Event event;

		while (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_WINDOWEVENT:
				{
					render();
				} break;
				case SDL_QUIT:
				{
					running = false;
					result = 1;
				} break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym){
						case SDLK_UP:
						{
							if (timer_option >= 300) break;
							timer_option++;
							render();
						} break;
						case SDLK_DOWN:
						{
							if (timer_option <= 0) break;
							timer_option--;
							render();
						} break;
					}
				} break;
				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
						{
							running = false;
							result = 1;
						} break;
						case SDLK_SPACE:
						{
							running = false;
						} break;
					}
				} break;
			}
		}
	}
}

void IntroMenu::render()
{
	const char *s1 = "Press SPACE to begin";
	const char *s2 = "Press ESCAPE to exit";
	const char *s3 = "Timer";
	char s4[4];

	snprintf(s4, sizeof(s4), "%i", timer_option);

	video->start_render();
	video->blit_background();
	video->render_text(220, 250, s1);
	video->render_text(220, 330, s2);
	video->render_text(220, 410, s3);
	video->render_text(540, 410, s4);
	video->finish_render();
}

Menu::Menu(Video *video)
{
	this->video = video;
	running = true;
	result = 0;
}

void Menu::event_loop()
{
	while (running){
		SDL_Event event;

		while (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_WINDOWEVENT:
				{
					render();
				} break;
				case SDL_QUIT:
				{
					running = false;
					result = 1;
				} break;
				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
						{
							running = false;
							result = 1;
						} break;
						case SDLK_SPACE:
						{
							running = false;
						} break;
					}
				} break;
			}
		}
	}
}

void Menu::render()
{
	video->start_render();
	video->blit_background();
	// render something
	video->finish_render();
}
