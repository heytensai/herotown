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
	void process_hero_inputs(const Uint8 *state, int heronum);
	void process_events();
	void move_hero(int i);
	void move_bomb(Bomb *bomb);
	void init_hero();
	void init_blocks();
	void add_block(int x, int y);

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
