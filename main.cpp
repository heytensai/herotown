#include "game.h"

Game *game;

void draw_background()
{
	if (game->sprite.moving()){
		if (!game->sprite.can_move('x')){
			if (game->sprite.motion.movement.x < 0){
				game->sprite.location.x -= game->sprite.motion.movement.x;
				game->background.movement.x = -Sprite::step;
				game->background.active = 1;
			}
			else if (game->sprite.motion.movement.x > 0){
				game->sprite.location.x -= game->sprite.motion.movement.x;
				game->background.movement.x = Sprite::step;
				game->background.active = 1;
			}
		}

		if (!game->sprite.can_move('y')){
			if (game->sprite.motion.movement.y < 0){
				game->sprite.location.y -= game->sprite.motion.movement.y;
				game->background.movement.y = -Sprite::step;
				game->background.active = 1;
			}
			else if (game->sprite.motion.movement.y > 0){
				game->sprite.location.y -= game->sprite.motion.movement.y;
				game->background.movement.y = Sprite::step;
				game->background.active = 1;
			}
		}
	}
	else{
		game->background.movement.x = 0;
		game->background.movement.y = 0;
		game->background.active = 0;
	}

	// move grid
	if (game->background.active){
		game->grid_base.y += game->background.movement.y;
		game->grid_base.x += game->background.movement.x;
	}

	// update texture
	for (int x=0; x<game->width; x++){
		for (int y=0; y<game->height; y++){
			game->video->set_pixel(
				x, y,
				((x + game->grid_base.x) & 0xff),
				((y + game->grid_base.y) & 0xff),
				((y + game->grid_base.y + x + game->grid_base.x) & 0xff)
			);
		}
	}
}

void draw_sprite()
{
	// move sprite (if sprite is in bounds)
	if (game->sprite.motion.active){
		if (game->sprite.can_move('x')){
			game->sprite.location.x += game->sprite.motion.movement.x;
		}

		if (game->sprite.can_move('y')){
			game->sprite.location.y += game->sprite.motion.movement.y;
		}
	}

	// draw sprite
	int offset;
	for (int x=-Sprite::size; x<Sprite::size; x++){
		for (int y=-Sprite::size; y<Sprite::size; y++){
			game->video->set_pixel(
				game->sprite.location.x + x,
				game->sprite.location.y + y,
				0xff,
				0xff,
				0xff
			);
		}
	}
}

int main(int argc, char **argv)
{
	game = new Game(WIDTH, HEIGHT);

	while (game->running){

		// process events
		game->process_events();

		draw_background();

		draw_sprite();

		// blit pixels
		game->video->blit();
	}

	game->sound.destroy();
	game->video->quit();
}
