#include <stdlib.h>
#include "matrices.h"
#include "tetro.h"
#include "block.h"

Tetro::Tetro(sf::Color c, BlockList blocks) : _col(3), _row(5), _c(c),
	_blocks(blocks) {}

const uint8_t Tetro::getColumn() const {
	return this->_col;
}

const uint8_t Tetro::getRow() const {
	return this->_row;
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
		_blocks[i]._x = -1 * rot * _blocks[i]._y;
		_blocks[i]._y = rot * temp;
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

