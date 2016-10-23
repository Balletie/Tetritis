#include <stdlib.h>
#include <SFML/Graphics/Transform.hpp>
#include "tetro.h"

sf::Color tetro_colors[NUM_TETRO] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan, sf::Color::White };

Block::Block(sf::Color c) : _c(c)
{}

void Block::drawVertices(sf::Vertex *quad, sf::Transform& transform) const {
	sf::Vector2f final_x_y = transform.transformPoint((float)_x, -(float)_y);
	quad[0].position = final_x_y;
	quad[1].position = final_x_y + sf::Vector2f(CELL_WIDTH_HEIGHT, 0);
	quad[2].position = final_x_y + sf::Vector2f(CELL_WIDTH_HEIGHT, CELL_WIDTH_HEIGHT);
	quad[3].position = final_x_y + sf::Vector2f(0, CELL_WIDTH_HEIGHT);
	for (int j = 0; j < 4; j++) quad[j].color = this->getColor();
}

const sf::Color Block::getColor() const {
	return this->_c;
}

Tetro::Tetro(tet_type t) : _col(3), _row(5), _t(t), _blocks(4, Block(tetro_colors[t])) {
	switch(t) {
		case TETRO_ZEE:
			initZee();
			break;
		case TETRO_SQUARE:
			initSquare();
			break;
		case TETRO_ESS:
			initEss();
			break;
		case TETRO_ELL:
			initEll();
			break;
		case TETRO_JAY:
			initJay();
			break;
		case TETRO_TEE:
			initTee();
			break;
		case TETRO_LONG:
			initLong();
			break;
		default:
			break;
	}
}

const int8_t Tetro::getFinalX(const Block& b) const {
	return (int8_t)this->_col + b._x;
}

const int8_t Tetro::getFinalY(const Block& b) const {
	return (int8_t)this->_row - b._y;
}

Tetro Tetro::randomTetro() {
	tet_type next = (tet_type) (rand() % NUM_TETRO);
	return Tetro(next);
}

void Tetro::rotateRight() {
	if (this->_t == TETRO_SQUARE) return;
	for (int i = 0; i < 4; i++) {
		int8_t temp = _blocks[i]._x;
		_blocks[i]._x = _blocks[i]._y;
		_blocks[i]._y = -1 * temp;
	}
}

void Tetro::rotateLeft() {
	if (this->_t == TETRO_SQUARE) return;
	for (int i = 0; i < 4; i++) {
		int8_t temp = _blocks[i]._x;
		_blocks[i]._x = -1 * _blocks[i]._y;
		_blocks[i]._y = temp;
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
		sf::Transform t = states.transform;
		const float final_x = CELL_WIDTH_HEIGHT * this->_col;
		const float final_y = CELL_WIDTH_HEIGHT * this->_row;
		_blocks[i].drawVertices(quad, t.translate(final_x, final_y)
			.scale(CELL_WIDTH_HEIGHT, CELL_WIDTH_HEIGHT));
	}
	target.draw(vertices, states);
}

void Tetro::initSquare() {
	_blocks[0]._x = 0, _blocks[0]._y = 0;
	_blocks[1]._x = 1,  _blocks[1]._y = 0;
	_blocks[2]._x = 0,  _blocks[2]._y = 1;
	_blocks[3]._x = 1,  _blocks[3]._y = 1;
}

void Tetro::initZee() {
	_blocks[0]._x = -1, _blocks[0]._y = 0;
	_blocks[1]._x = 0,  _blocks[1]._y = 0;
	_blocks[2]._x = 0,  _blocks[2]._y = -1;
	_blocks[3]._x = 1,  _blocks[3]._y = -1;
}

void Tetro::initEss() {
	_blocks[0]._x = 1, _blocks[0]._y = 0;
	_blocks[1]._x = 0,  _blocks[1]._y = 0;
	_blocks[2]._x = 0,  _blocks[2]._y = -1;
	_blocks[3]._x = -1,  _blocks[3]._y = -1;
}

void Tetro::initEll() {
	_blocks[0]._x = 0, _blocks[0]._y = 1;
	_blocks[1]._x = 0,  _blocks[1]._y = 0;
	_blocks[2]._x = 0,  _blocks[2]._y = -1;
	_blocks[3]._x = 1,  _blocks[3]._y = -1;
}

void Tetro::initJay() {
	_blocks[0]._x = 0, _blocks[0]._y = 1;
	_blocks[1]._x = 0,  _blocks[1]._y = 0;
	_blocks[2]._x = 0,  _blocks[2]._y = -1;
	_blocks[3]._x = -1,  _blocks[3]._y = -1;
}

void Tetro::initTee() {
	_blocks[0]._x = -1, _blocks[0]._y = 0;
	_blocks[1]._x = 0,  _blocks[1]._y = 0;
	_blocks[2]._x = 1,  _blocks[2]._y = 0;
	_blocks[3]._x = 0,  _blocks[3]._y = 1;
}

void Tetro::initLong() {
	_blocks[0]._x = 0, _blocks[0]._y = 1;
	_blocks[1]._x = 0,  _blocks[1]._y = 0;
	_blocks[2]._x = 0,  _blocks[2]._y = -1;
	_blocks[3]._x = 0,  _blocks[3]._y = -2;
}
