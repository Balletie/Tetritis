#include <stdlib.h>
#include "matrices.h"
#include "tetro.h"
#include "block.h"

Tetro::Tetro(sf::Color c, BlockList blocks, Tetro::WallKickOffsetRow (*wk_offsets)[4])
	: _col(4), _row(1), _orntn(ZERO), _wall_kick_offsets(wk_offsets), _c(c)
	, _blocks(blocks) {}

const sf::Color Tetro::Cyan(49, 199, 239);
const sf::Color Tetro::Yellow(247,211,8);
const sf::Color Tetro::Purple(173,77,156);
const sf::Color Tetro::Green(66,182,66);
const sf::Color Tetro::Red(239,32,41);
const sf::Color Tetro::Blue(90,101,173);
const sf::Color Tetro::Orange(239,121,33);

/* These tables have been retrieved from
 * https://tetris.wiki/SRS#How_Guideline_SRS_Really_Works. The tables for the I
 * and O block also take into account the "wobble" when rotating (see first
 * columns). This therefore removes the need for a vector member variable which
 * translates the block origin (e.g. bottom left corner for O) to the true
 * origin (the middle of the O block, which does not mark an actual block but
 * is inbetween blocks).
 */
Tetro::WallKickOffsetRow Tetro::jlstz_offsets[4] {
	{{ 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}},
	{{ 0, 0}, {+1, 0}, {+1,-1}, { 0,+2}, {+1,+2}},
	{{ 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0}},
	{{ 0, 0}, {-1, 0}, {-1,-1}, { 0,+2}, {-1,+2}}
};
Tetro::WallKickOffsetRow Tetro::i_offsets[4] {
	{{ 0, 0}, {-1, 0}, {+2, 0}, {-1, 0}, {+2, 0}},
	{{-1, 0}, { 0, 0}, { 0, 0}, { 0,+1}, { 0,-2}},
	{{-1,+1}, {+1,+1}, {-2,+1}, {+1, 0}, {-2, 0}},
	{{ 0,+1}, { 0,+1}, { 0,+1}, { 0,-1}, { 0,+2}}
};
Tetro::WallKickOffsetRow Tetro::o_offsets[4] {
	{{ 0, 0}},
	{{ 0,-1}},
	{{-1,-1}},
	{{-1, 0}}
};

uint8_t Tetro::getColumn() const {
	return this->_col;
}

uint8_t Tetro::getRow() const {
	return this->_row;
}

int8_t Tetro::getFinalX(const TetroBlock& b) const {
	return (int8_t)this->_col + b._x;
}

int8_t Tetro::getFinalY(const TetroBlock& b) const {
	return (int8_t)this->_row - b._y;
}

sf::Color Tetro::getColor() const {
	return this->_c;
}

void Tetro::translate(int8_t x, int8_t y) {
	this->_col += x;
	this->_row -= y;
}

Tetro::WallKickOffset Tetro::getWallKickOffset(uint8_t idx) {
	return (*_wall_kick_offsets)[_orntn][idx];
}

Tetro::WallKickOffset Tetro::getWallKickTranslation(rotation rot, uint8_t idx) {
	Tetro::WallKickOffset current_offset = getWallKickOffset(idx);
	Tetro::WallKickOffset next_offset = (*_wall_kick_offsets)[_orntn + rot][idx];

	return std::make_pair(
		current_offset.first - next_offset.first,
		current_offset.second - next_offset.second
	);
}

Tetro::WallKickTranslation Tetro::rotate(rotation rot, uint8_t kick_test_num) {
	for (int i = 0; i < 4; i++) {
		int8_t temp = _blocks[i]._x;
		_blocks[i]._x = rot * _blocks[i]._y;
		_blocks[i]._y = -1 * rot * temp;
	}
	Tetro::WallKickTranslation xy = getWallKickTranslation(rot, kick_test_num);
	_orntn += rot;
	translate(xy.first, xy.second);
	return xy;
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
		_blocks[i].drawVertices(quad, this->getColor());
	}
	const float final_x = this->_col;
	const float final_y = this->_row;
	states.transform.translate(final_x, final_y);
	target.draw(vertices, states);
}

