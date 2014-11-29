#include "game.h"

int width = WIDTH;
int height = HEIGHT;
int pitch = width * 2;

Game game;

void draw_background()
{
	// move grid
	if (game.vertical.active){
		game.grid_base.y += game.vertical.movement.y;
	}
	if (game.horizontal.active){
		game.grid_base.x += game.horizontal.movement.x;
	}

	// update texture
	for (int x=0; x<width; x++){
		for (int y=0; y<height; y++){
			int offset = pitch_offset(x, y);
			game.video.pixels[offset] = game.video.map_rgba(((x + game.grid_base.x) & 0xff), ((y + game.grid_base.y) & 0xff), ((y + game.grid_base.y + x + game.grid_base.x) & 0xff), 0);
		}
	}
}

void draw_sprite()
{
	// move sprite (if sprite is in bounds)
	if (game.sprite.motion.active){
		if (game.sprite.can_move('x')){
			game.sprite.location.x += game.sprite.motion.movement.x;
		}

		if (game.sprite.can_move('y')){
			game.sprite.location.y += game.sprite.motion.movement.y;
		}
	}

	// draw sprite
	int offset;
	for (int x=-Sprite::size; x<Sprite::size; x++){
		for (int y=-Sprite::size; y<Sprite::size; y++){
			offset = pitch_offset(game.sprite.location.x + x, game.sprite.location.y + y);
			game.video.pixels[offset] = game.video.map_rgba(0xff, 0xff, 0xff, 0);
		}
	}
}

void graphics_init()
{
	game.vertical.active = 0;
	game.vertical.movement.x = 0;
	game.vertical.movement.y = 0;

	game.horizontal.active = 1;
	game.horizontal.movement.x = 1;
	game.horizontal.movement.y = 0;

	game.sprite.location.x = 100;
	game.sprite.location.y = 100;

	game.sprite.motion.active = 0;
	game.sprite.motion.movement.x = 0;
	game.sprite.motion.movement.y = 0;
}

int main(int argc, char **argv)
{
	game.video.init();
	game.video.create_window();
	game.video.create_pixels();

	game.sound.init();

	graphics_init();

	while (game.running){

		// process events
		game.process_events();

		draw_background();

		draw_sprite();

		// blit pixels
		game.video.blit();
	}

	game.sound.destroy();
	game.video.quit();
}
