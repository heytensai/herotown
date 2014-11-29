#ifndef POINT_H
#define POINT_H

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	int active;
	point_t movement;
} movement_t;

#endif /* POINT_H */
