#include "movements.h"

orientation operator+(const orientation& ort, const rotation& rot) {
	int r = ((uint8_t) ort + (int8_t) rot) % 4;
	return (orientation) (r < 0 ? r + 4 : r);
}

orientation& operator+=(orientation& ort, const rotation& rot) {
	ort = ort + rot;
	return ort;
}
