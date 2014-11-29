#include "sprite.h"

bool Sprite::moving()
{
	return (motion.movement.x != 0) && (motion.movement.y != 0);
}
