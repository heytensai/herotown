#ifndef JUMPYGAME_H
#define JUMPYGAME_H

#include "game.h"
#include "hero.h"

class JumpyGame : public Game
{
private:
	Hero *hero[2];
	bool run_once;
	bool running;
	void render();
	void process_inputs();
	void process_events();
	void move_hero();
	void init_hero();

public:
	JumpyGame(Video *video, Sound *sound);
	~JumpyGame();

	void start();
	void end();
	bool intro_screen();
	void score_screen();
	void main_loop();
};

#endif /* JUMPYGAME_H */
