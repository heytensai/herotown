#include "game.h"
#include "sprite.h"
#include "sdl.h"

int global_running = 1;
int width = WIDTH;
int height = HEIGHT;
int pitch = width * 2;

SDL sdl;
point_t grid_base;
movement_t vertical;
movement_t horizontal;
Sprite sprite;

void draw_background()
{
	// move grid
	if (vertical.active){
		grid_base.y += vertical.movement.y;
	}
	if (horizontal.active){
		grid_base.x += horizontal.movement.x;
	}

	// update texture
	for (int x=0; x<width; x++){
		for (int y=0; y<height; y++){
			int offset = pitch_offset(x, y);
			sdl.pixels[offset] = sdl.map_rgba(((x + grid_base.x) & 0xff), ((y + grid_base.y) & 0xff), ((y + grid_base.y + x + grid_base.x) & 0xff), 0);
		}
	}
}

void draw_sprite()
{
	// move sprite (if sprite is in bounds)
	if (sprite.motion.active){
		if (sprite.can_move('x')){
			sprite.location.x += sprite.motion.movement.x;
		}

		if (sprite.can_move('y')){
			sprite.location.y += sprite.motion.movement.y;
		}
	}

	// draw sprite
	int offset;
	for (int x=-Sprite::size; x<Sprite::size; x++){
		for (int y=-Sprite::size; y<Sprite::size; y++){
			offset = pitch_offset(sprite.location.x + x, sprite.location.y + y);
			sdl.pixels[offset] = sdl.map_rgba(0xff, 0xff, 0xff, 0);
		}
	}
}

void graphics_init()
{
	vertical.active = 0;
	vertical.movement.x = 0;
	vertical.movement.y = 0;

	horizontal.active = 1;
	horizontal.movement.x = 1;
	horizontal.movement.y = 0;

	sprite.location.x = 100;
	sprite.location.y = 100;

	sprite.motion.active = 0;
	sprite.motion.movement.x = 0;
	sprite.motion.movement.y = 0;
}

int main(int argc, char **argv)
{
	sdl.init();
	sdl.create_window();
	sdl.create_pixels();

	graphics_init();

	while (global_running){

		// process events
		sdl.process_events();

		draw_background();

		draw_sprite();

		// blit pixels
		sdl.blit();
	}

	sdl.quit();
}
