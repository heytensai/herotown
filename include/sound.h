#ifndef SOUND_H
#define SOUND_H

#include "SDL2/SDL_mixer.h"

class Sound
{
private:
	static const int SOUND_MAX = 16;
	Mix_Chunk *sounds[SOUND_MAX];
	int sound_count;

public:
	typedef enum
	{
		SOUND_BEEP,
		SOUND_DING1,
		SOUND_DING2,
		SOUND_EXPLODE,
		SOUND_TICKING,
	} sound_t;

	Sound();
	void destroy();
	void init();
	void play(int soundId);
	void load(int soundId, const char *file);
	void unload(int soundId);
};

#endif /* SOUND_H */
