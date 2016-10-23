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

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

  private:
	void deleteRowsAsNeeded(uint8_t, uint8_t);

	size_t size() const;

	uint8_t toRowVectorIndex(const uint8_t row) const {
		return BOARD_HEIGHT - row - 1;
	}

	bool isOutOfBounds(const Tetro&, const Block&) const;

	bool containsRow(const uint8_t row) const {
		return toRowVectorIndex(row) < _rows.size();
	}

	bool contains(const uint8_t, const uint8_t) const;

	std::vector<Row> _rows;
	typedef std::vector<Row>::const_iterator const_iterator;
};
