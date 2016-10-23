#ifndef BOARD_H
#define BOARD_H

#include "tetro.h"
#include <map>

const uint8_t BOARD_WIDTH = 10;
const uint8_t BOARD_HEIGHT = 22;

// Rows are sorted maps from column index to block.
typedef std::map<uint8_t, Block> Row;

class Board : public sf::Drawable {
  public:
	Board();
	bool collides(const Tetro&) const;
	void record(const Tetro&);

	/*
	 * Deletes the rows that are full between the given range, which is
	 * (inclusive, exclusive].
	 */
	void deleteFullRows(uint8_t, uint8_t);

	typedef std::vector<Row>::const_iterator const_iterator;

	const_iterator begin() const {
		return this->_rows.begin();
	}

	const_iterator end() const {
		return this->_rows.end();
	}

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

  private:
	size_t size() const;

	/*
	 * The row indices of the vector are inverted with respect to the row
	 * locations as shown on screen. So row 0 represents the bottom-most row in
	 * the vector, and row BOARD_HEIGHT - 1 represents the bottom-most row as
	 * shown on screen. Hence this function.
	 */
	uint8_t toRowVectorIndex(const uint8_t row) const {
		return BOARD_HEIGHT - row - 1;
	}

	bool rowFull(Row r) {
		return r.size() == BOARD_WIDTH;
	}

	bool isOutOfBounds(const Tetro&, const Block&) const;

	bool containsRow(const uint8_t row) const {
		return toRowVectorIndex(row) < _rows.size();
	}

	bool contains(const uint8_t, const uint8_t) const;

	std::vector<Row> _rows;
};

#endif /* BOARD_H */
