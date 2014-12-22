#ifndef JUMPYGAME_H
#define JUMPYGAME_H

#include "game.h"
#include "hero.h"

class JumpyGame : public Game
{
private:
	static const int BLOCKS = 64;
	Block *blocks[BLOCKS];
	Hero *hero[2];
	bool run_once;
	bool running;
	velocity_t gravity;

	void render();
	void process_inputs();
	void process_events();
	void move_hero();
	void init_hero();
	void init_blocks();

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
