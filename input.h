#ifndef INPUT_H
#define INPUT_H

#define INPUT_STATE_MOVE_UP    0x0001
#define INPUT_STATE_MOVE_DOWN  0x0002
#define INPUT_STATE_MOVE_LEFT  0x0004
#define INPUT_STATE_MOVE_RIGHT 0x0008
#define INPUT_STATE_DROP_COIN  0x0010
#define INPUT_STATE_QUIT       0x0020

typedef int input_state_t;

#endif /* INPUT_H */
