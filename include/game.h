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

	void init_controller();

public:

	Game(Video *video, Sound *sound);
	virtual ~Game();

	// Override these
	virtual void start() =0;
	virtual void end() =0;
	virtual bool intro_screen() =0;
	virtual void score_screen() =0;
	virtual void main_loop() =0;
};

#endif /* GAME_H */
