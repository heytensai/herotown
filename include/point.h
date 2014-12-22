#ifndef POINT_H
#define POINT_H

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	point_t top_left;
	point_t bottom_right;
} bounding_box_t;

typedef struct {
	int active;
	point_t movement;
} movement_t;

typedef struct {
	float direction;
	float speed;
} velocity_t;

#endif /* POINT_H */
