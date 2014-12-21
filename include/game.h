#ifndef GAME_H
#define GAME_H

#include "video.h"
#include "sound.h"

class Game
{
protected:
	static const int MAX_JOYDEV = 4;
	Video *video;
	Sound *sound;
	bool use_joy;
	SDL_Joystick *joy[MAX_JOYDEV]; // 4 joydevs seems enough

public:

	Game(Video *video, Sound *sound);
	void init_controller();

	// Override these
	void start();
	void end();
	bool intro_screen();
	void score_screen();
	void main_loop();
};

#endif /* GAME_H */
