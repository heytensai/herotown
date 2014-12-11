#include "menu.h"

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
	const char *s1 = "Press SPACE to begin";
	const char *s2 = "Press ESCAPE to exit";

	video->start_render();
	video->blit_background();
	video->render_text(220, 250, s1);
	video->render_text(220, 330, s2);
	video->finish_render();
}
