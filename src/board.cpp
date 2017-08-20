#include <numeric>
#include <SFML/Graphics/VertexArray.hpp>
#include "matrices.h"
#include "board.h"

Board::Board()
{}

bool Board::collides(const Tetro& t) const {
	for (Tetro::const_iterator bs = t.begin(); bs != t.end(); bs++) {
		if (this->isOutOfBottomBounds(t, *bs)) return true;

		if (!this->contains(t.getFinalY(*bs), t.getFinalX(*bs)))
			continue;
		else
			return true;
	}
	return false;
}

bool Board::isOutOfSideBounds(const Tetro& t) const {
	for (Tetro::const_iterator bs = t.begin(); bs != t.end(); bs++) {
		if (this->isOutOfSideBounds(t, *bs)) return true;
	}
	return false;
}

std::pair<uint8_t, uint8_t> Board::record(const Tetro& t) {
	uint8_t min_y = BOARD_HEIGHT;
	uint8_t max_y = 0;
	for (Tetro::const_iterator bs = t.begin(); bs != t.end(); bs++) {
		if (this->isOutOfBottomBounds(t, *bs)) continue;

		uint8_t fin_x = (uint8_t) t.getFinalX(*bs);
		uint8_t fin_y = (uint8_t) t.getFinalY(*bs);
		min_y = std::min(min_y, fin_y);
		max_y = std::max(max_y, fin_y);

		// Grow as needed
		if (!this->containsRow(fin_y)) {
			_rows.resize(toRowVectorIndex(fin_y) + 1);
		}

		_rows[toRowVectorIndex(fin_y)].emplace(fin_x, BoardBlock(t.getColor()));
	}
	return std::make_pair(min_y, max_y);
}

std::map<uint8_t, Board> Board::deleteFullRows(uint8_t from, uint8_t to) {
	std::map<uint8_t, Board> removedBoards;
	std::vector<Row> cur_removed;
	int8_t cur_idx = -1;
	uint8_t accum_removed = 0;

	uint8_t tempfrom = from;
	from = toRowVectorIndex(to) + 1;
	to = toRowVectorIndex(tempfrom) + 1;

	Board::const_iterator start = this->begin() + from;
	for (Board::const_iterator it = start; it < this->begin() + to - accum_removed;) {
		if (rowFull(*it)) {
			if (cur_idx < 0)
				cur_idx = std::distance(start, it) + from + accum_removed;
			cur_removed.push_back(*it);
			it = _rows.erase(it);
			accum_removed++;
		} else {
			if (cur_idx >= 0) {
				removedBoards.emplace((uint8_t) cur_idx, Board(std::move(cur_removed)));
				cur_removed = std::vector<Row>();
				cur_idx = -1;
			}
			it++;
		}
	}
	if (cur_idx >= 0)
		removedBoards.emplace((uint8_t) cur_idx, Board(std::move(cur_removed)));
	return removedBoards;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if(_rows.empty()) return;

	sf::VertexArray vertices(sf::Quads, 4*this->size());
	Board::const_iterator it = this->begin();

	for (uint8_t row = 0, i = 0; it != this->end(); ++it, row++) {
		for (Row::const_iterator bl_it = it->begin(); bl_it != it->end(); ++bl_it, i++) {
			sf::Vertex* quad = &vertices[4*i];
			uint8_t col  = bl_it->first;
			BoardBlock bl = bl_it->second;
			uint16_t x = col;
			uint16_t y = BOARD_HEIGHT - row - 1;
			bl.drawVertices(quad, x, y);
		}
	}
	target.draw(vertices, states);
}

/* Private functions */
bool Board::isOutOfBounds(const Tetro& t, const TetroBlock& b) const {
	return isOutOfSideBounds(t, b) || isOutOfBottomBounds(t, b);
}

bool Board::isOutOfSideBounds(const Tetro& t, const TetroBlock& b) const {
	const int8_t fin_x = t.getFinalX(b);
	if (fin_x < 0 || fin_x >= BOARD_WIDTH)	return true;

	return false;
}

bool Board::isOutOfBottomBounds(const Tetro& t, const TetroBlock& b) const {
	const int8_t fin_y = t.getFinalY(b);
	if (fin_y < 0 || fin_y >= BOARD_HEIGHT)	return true;

	return false;
}

size_t Board::height() const {
	return _rows.size();
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
