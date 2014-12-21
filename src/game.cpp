#include "game.h"

Game::Game(Video *video, Sound *sound)
{
	this->video = video;
	this->sound = sound;

	//init_controller();

	use_joy = false;

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
}

Game::~Game()
{
}

void Game::init_controller()
{
	for (int i=0; i<MAX_JOYDEV; i++){
		joy[i] = NULL;
	}

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	fprintf(stdout, "joy=%d\n", SDL_NumJoysticks());
	if (SDL_NumJoysticks() > 0){
		use_joy = true;
		for (int i=0; i<SDL_NumJoysticks(); i++){
			joy[i] = SDL_JoystickOpen(i);
			printf("Name: %s\n", SDL_JoystickNameForIndex(i));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy[i]));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy[i]));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy[i]));
			if (SDL_IsGameController(i)){
				printf("Is a game controller\n");
			}
			else{
				printf("Is NOT a game controller\n");
			}
		}
	}
	else{
		use_joy = true;
	}
}

