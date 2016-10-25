#include <stdlib.h>
#include "matrices.h"
#include "tetro.h"
#include "block.h"

Tetro::Tetro(sf::Color c, float c_x, float c_y, BlockList blocks)
	: _col(3), _row(5), _center_x(c_x), _center_y(c_y), _c(c),
	_blocks(blocks) {}

Tetro::Tetro(sf::Color c, BlockList blocks) : Tetro (c, 0.f, 0.f, blocks) {}

const sf::Color Tetro::Cyan(49, 199, 239);
const sf::Color Tetro::Yellow(247,211,8);
const sf::Color Tetro::Purple(173,77,156);
const sf::Color Tetro::Green(66,182,66);
const sf::Color Tetro::Red(239,32,41);
const sf::Color Tetro::Blue(90,101,173);
const sf::Color Tetro::Orange(239,121,33);

const uint8_t Tetro::getColumn() const {
	return this->_col;
}

const uint8_t Tetro::getRow() const {
	return this->_row;
}

const float Tetro::getCenterX() const {
	return this->_center_x;
}

const float Tetro::getCenterY() const {
	return this->_center_y;
}

const int8_t Tetro::getFinalX(const TetroBlock& b) const {
	return (int8_t)this->_col + b._x;
}

const int8_t Tetro::getFinalY(const TetroBlock& b) const {
	return (int8_t)this->_row - b._y;
}

sf::Color Tetro::getColor() const {
	return this->_c;
}

void Tetro::rotate(rotation rot) {
	for (int i = 0; i < 4; i++) {
		int8_t temp = _blocks[i]._x;
		_blocks[i]._x = -1 * rot * _blocks[i]._y + _center_x + _center_y * rot;
		_blocks[i]._y = rot * temp + _center_y - _center_x * rot;
	}
}

void Tetro::move(direction d) {
	switch(d) {
		case DIR_DOWN:
			this->_row++;
		break;
		case DIR_LEFT:
			this->_col--;
		break;
		case DIR_RIGHT:
			this->_col++;
		break;
		default:
		break;
	}
}

void Tetro::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::VertexArray vertices(sf::Quads, 4*4);
	for (int i = 0; i < 4; i++) {
		sf::Vertex* quad = &vertices[4*i];
		sf::Transform t = scale_mat(CELL_WIDTH_HEIGHT);
		_blocks[i].drawVertices(quad, t, this->getColor());
	}
	const float final_x = CELL_WIDTH_HEIGHT * this->_col;
	const float final_y = CELL_WIDTH_HEIGHT * this->_row;
	states.transform.translate(final_x, final_y);
	target.draw(vertices, states);
}

