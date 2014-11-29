#ifndef SOUND_H
#define SOUND_H

#include "SDL2/SDL_mixer.h"

class Sound
{
public:
	void init();
	void play(int);
};

#endif /* SOUND_H */
