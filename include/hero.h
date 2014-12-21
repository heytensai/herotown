#ifndef HERO_H
#define HERO_H
#include "sprite.h"

#define HERO_MOVE_LEFT  0x01
#define HERO_MOVE_RIGHT 0x02
#define HERO_MOVE_UP    0x04
#define HERO_MOVE_DOWN  0x08

#define HERO_ACTION_COIN 0x01
#define HERO_ACTION_BOMB 0x01

class Hero : public Sprite
{
private:

public:
	static const int STEP = 5;
	int direction;
	int action;
	int score;
	Uint32 last_bomb_added;
	
	Hero(Video *video, int width, int height);
	void subtract_coins(int count);

};

class Yoshi : public Hero
{
public:
	Yoshi(Video *video);
};

class Mario : public Hero
{
public:
	Mario(Video *video);
};

#endif /* HERO_H */
