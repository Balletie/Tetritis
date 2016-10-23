#include <numeric>
#include <SFML/Graphics/VertexArray.hpp>
#include "board.h"

Board::Board()
{}

bool Board::collides(const Tetro& t) const {
	for (Tetro::const_iterator bs = t.begin(); bs != t.end(); bs++) {
		if (this->isOutOfBounds(t, *bs)) return true;

		if (!this->contains(t.getFinalY(*bs), t.getFinalX(*bs)))
			continue;
		else
			return true;
	}
	return false;
}

void Board::record(const Tetro& t) {
	uint8_t min_y = BOARD_HEIGHT;
	uint8_t max_y = 0;
	for (Tetro::const_iterator bs = t.begin(); bs != t.end(); bs++) {
		if (this->isOutOfBounds(t, *bs)) continue;

		uint8_t fin_x = (uint8_t) t.getFinalX(*bs);
		uint8_t fin_y = (uint8_t) t.getFinalY(*bs);
		min_y = std::min(min_y, fin_y);
		max_y = std::max(max_y, fin_y);

		// Grow as needed
		if (!this->containsRow(fin_y)) {
			_rows.resize(toRowVectorIndex(fin_y) + 1);
		}

		_rows[toRowVectorIndex(fin_y)].insert({fin_x, *bs});
	}
	this->deleteFullRows(min_y, max_y + 1);
}

void Board::deleteFullRows(uint8_t from, uint8_t to) {
	uint8_t tempfrom = from;
	from = toRowVectorIndex(to) + 1;
	to = toRowVectorIndex(tempfrom) + 1;
	for (Board::const_iterator it = this->begin() + from; it < this->begin() + to;) {
		if (rowFull(*it))
			it = _rows.erase(it);
		else
			it++;
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if(_rows.empty()) return;

	sf::VertexArray vertices(sf::Quads, 4*this->size());
	Board::const_iterator it = this->begin();

	for (uint8_t row = 0, i = 0; it != this->end(); ++it, row++) {
		for (Row::const_iterator bl_it = it->begin(); bl_it != it->end(); ++bl_it, i++) {
			sf::Vertex* quad = &vertices[4*i];
			sf::Transform t = states.transform;
			uint8_t col  = bl_it->first;
			Block bl = bl_it->second;
			uint16_t x = CELL_WIDTH_HEIGHT * col;
			uint16_t y = CELL_WIDTH_HEIGHT * (BOARD_HEIGHT - row - 1);
			bl.drawVertices(quad, t.translate(x,y).scale(0, 0));
		}
	}
	target.draw(vertices);
}

/* Private functions */
bool Board::isOutOfBounds(const Tetro& t, const Block& b) const {
	const int8_t fin_y = t.getFinalY(b);
	if (fin_y < 1 || fin_y >= BOARD_HEIGHT)	return true;

	const int8_t fin_x = t.getFinalX(b);
	if (fin_x < 0 || fin_x >= BOARD_WIDTH)	return true;

	return false;
}

size_t Board::size() const {
	return std::accumulate(this->begin(), this->end(), 0, [](size_t a, Row r) {
		return a + r.size();
	});
}

bool Board::contains(const uint8_t row, const uint8_t column) const {
	if (!containsRow(row))	return false;

	Row r = _rows[toRowVectorIndex(row)];
	return r.find(column) != r.end();
}
