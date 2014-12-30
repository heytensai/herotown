#ifndef EXPLORYGAME_H
#define EXPLORYGAME_H

#include "game.h"
#include "tile.h"
#include "hero.h"

class ExploryGame : public Game
{
public:
	static const int TILE_X = 33;
	static const int TILE_Y = 13;

	ExploryGame(Video *video, Sound *sound);
	~ExploryGame();

	void start();
	void end();
	bool intro_screen();
	void score_screen();
	void main_loop();

private:
	Hero *hero;
	bool running;
	bool exit_requested;
	Uint32 *tile_map;
	Tile *tile;

	void update_world();
	void process_inputs();
	void process_events();
	void render();
	void render_map();
	void load_tile_maps();

};

#endif /* EXPLORYGAME_H */
