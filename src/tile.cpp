#include "tile.h"

Tile::Tile(Video *video)
	: Sprite(video, 32, 48)
{
	load_image(Sprite::DIR_NONE, "resources/tile0.png");
}
