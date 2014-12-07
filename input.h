#ifndef INPUT_H
#define INPUT_H

#define INPUT_STATE_HERO1_MOVE_UP    0x0001
#define INPUT_STATE_HERO1_MOVE_DOWN  0x0002
#define INPUT_STATE_HERO1_MOVE_LEFT  0x0004
#define INPUT_STATE_HERO1_MOVE_RIGHT 0x0008
#define INPUT_STATE_HERO2_MOVE_UP    0x0010
#define INPUT_STATE_HERO2_MOVE_DOWN  0x0020
#define INPUT_STATE_HERO2_MOVE_LEFT  0x0040
#define INPUT_STATE_HERO2_MOVE_RIGHT 0x0080
#define INPUT_STATE_HERO1_DROP_COIN  0x0100
#define INPUT_STATE_HERO2_DROP_COIN  0x0200
#define INPUT_STATE_QUIT             0x0400

typedef int input_state_t;

#endif /* INPUT_H */