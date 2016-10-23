#include <stdint.h>
#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>

const uint8_t CELL_WIDTH_HEIGHT = 16;

enum direction : uint8_t {
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

enum tet_type : uint8_t {
	TETRO_SQUARE,
	TETRO_ZEE,
	TETRO_ESS,
	TETRO_ELL,
	TETRO_JAY,
	TETRO_TEE,
	TETRO_LONG,

	TETRO_INVALID,
	NUM_TETRO = TETRO_INVALID
};

struct Block {
  protected:
	friend class Tetro;
	sf::Color _c;

	// The x and y coordinates are relative to the center of a tetrominos.
	int8_t _x;
	int8_t _y;

  public:
	Block(sf::Color);
	const sf::Color getColor() const;
};

class Tetro : public sf::Drawable {
  public:
	Tetro(sf::Color, tet_type);

	typedef std::vector<Block> BlockList;
	typedef BlockList::const_iterator const_iterator;

	const const_iterator begin() const {
		return this->_blocks.begin();
	};

	const const_iterator end() const {
		return this->_blocks.end();
	};

	const int8_t getFinalX(const Block&) const;
	const int8_t getFinalY(const Block&) const;

	void rotateRight();
	void rotateLeft();
	void move(direction);

	static Tetro randomTetro();

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

  private:
	void initSquare();
	void initZee();
	void initEss();
	void initEll();
	void initJay();
	void initTee();
	void initLong();

	uint8_t _col;
	uint8_t _row;
	tet_type _t;

	BlockList _blocks;
};
