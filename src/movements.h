#ifndef MOVEMENTS_H
#define MOVEMENTS_H
#include <stdint.h>

enum direction : int8_t {
	DIR_RIGHT = -1,
	DIR_DOWN,
	DIR_LEFT,
	NUM_DIRS = 3
};

/** Rotations in clockwise or counter-clockwise direction. */
enum rotation : int8_t {
	CCW = -1,
	CW = 1
};


/** Orientations or rotation states. The fields are positioned in such a way
 *  that when adding modulo 4 with a rotation it yields the new rotation state.
 */
enum orientation : uint8_t {
	ZERO,
	R,
	TWO,
	L
};

orientation operator+(const orientation&, const rotation&);

orientation& operator+=(orientation&, const rotation&);

#endif /* MOVEMENTS_H */
