#include "sound.h"

Sound::Sound()
{
	sound_count = -1;
	for (int i=0; i<SOUND_MAX; i++){
		sounds[i] = NULL;
	}
}

void Sound::destroy()
{
	while (sound_count > -1){
		unload(sound_count);
		sound_count--;
	}
	Mix_CloseAudio();
}

void Sound::init()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	load(SOUND_BEEP, "resources/beep.wav");
	load(SOUND_DING1, "resources/ding1.wav");
	load(SOUND_DING2, "resources/ding2.wav");
}

void Sound::load(int soundId, const char *file)
{
	if (sound_count + 1 == SOUND_MAX){
		fprintf(stderr, "no more sounds\n");
		return;
	}

	Mix_Chunk *s = Mix_LoadWAV(file);
	if (s == NULL){
		fprintf(stderr, "sound load failed\n");
		return;
	}

	sounds[++sound_count] = s;
}

void Sound::unload(int soundId)
{
	if (sounds[soundId] != NULL){
		Mix_FreeChunk(sounds[soundId]);
		sounds[soundId] = NULL;
	}
}

void Sound::play(int soundId)
{
	if (sounds[soundId] == NULL){
		return;
	}
	Mix_PlayChannel(-1, sounds[soundId], 0);
}
