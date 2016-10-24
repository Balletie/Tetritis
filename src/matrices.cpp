#include <cmath>
#include "matrices.h"

sf::Transform translation_mat(float x, float y) {
	return sf::Transform(
		1, 0, x,
		0, 1, y,
		0, 0, 1
	);
}

sf::Transform scale_mat(float x) {
	return sf::Transform(
		x, 0, 0,
		0, x, 0,
		0, 0, 1
	);
}

sf::Transform rotation_mat(float angle, float centerX, float centerY) {
	float rad = angle * M_PI / 180.f;
	float cos = std::cos(rad);
	float sin = std::sin(rad);
	return sf::Transform(
		cos, -sin, centerX * (1 - cos) + centerY * sin,
		sin,  cos, centerY * (1 - cos) - centerX * sin,
		0  ,  0  , 1
	);
}

