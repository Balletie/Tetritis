#ifndef BOARD_H
#define BOARD_H

#include <map>
#include "tetro.h"

const uint8_t BOARD_WIDTH = 10;
const uint8_t BOARD_HEIGHT = 23;

// Rows are sorted maps from column index to block.
typedef std::map<uint8_t, BoardBlock> Row;

class Board : public sf::Drawable {
  public:
	Board();
	bool collides(const Tetro&) const;
	bool isOutOfSideBounds(const Tetro&) const;

	/*
	 * Records the given tetrominos to the board. That is, it takes each of its
	 * blocks and stores it in the board as BoardBlocks. Returns the row index
	 * range that the tetrominos spanned.
	 */
	std::pair<uint8_t, uint8_t> record(const Tetro&);

	/*
	 * Deletes the rows that are full between the given range, which is
	 * (inclusive, exclusive]. Returns the deleted rows.
	 */
	std::map<uint8_t, Board> deleteFullRows(uint8_t, uint8_t);

	typedef std::vector<Row>::const_iterator const_iterator;

	const_iterator begin() const {
		return this->_rows.begin();
	}

	const_iterator end() const {
		return this->_rows.end();
	}

	Board subBoard(const_iterator begin, const_iterator end) const {
		return Board(std::vector<Row>(begin, end));
	}

	size_t height() const;
	size_t size() const;

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

  private:
	/* Private constructor for creating a board from a vector of rows. */
	Board(std::vector<Row>&& rows) : _rows(rows) {};

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

	bool isOutOfBounds(const Tetro&, const TetroBlock&) const;
	bool isOutOfSideBounds(const Tetro&, const TetroBlock&) const;
	bool isOutOfBottomBounds(const Tetro&, const TetroBlock&) const;

	bool containsRow(const uint8_t row) const {
		return toRowVectorIndex(row) < _rows.size();
	}

	bool contains(const uint8_t, const uint8_t) const;

	std::vector<Row> _rows;
};

#endif /* BOARD_H */
