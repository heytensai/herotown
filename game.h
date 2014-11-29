#ifndef GAME_H
#define GAME_H

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	int active;
	point_t movement;
} movement_t;

#endif /* GAME_H */
