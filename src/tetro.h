#ifndef TETRO_H
#define TETRO_H

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "block.h"

enum direction : int8_t {
	DIR_RIGHT = -1,
	DIR_DOWN,
	DIR_LEFT,
	NUM_DIRS = 3
};

enum rotation : int8_t {
	CW = -1,
	CCW = 1
};

class TetroFactory;

class Tetro : public sf::Drawable {
	public:
	typedef std::vector<TetroBlock> BlockList;
	typedef BlockList::const_iterator const_iterator;
	friend class TetroFactory;

	const const_iterator begin() const {
		return this->_blocks.begin();
	};

	const const_iterator end() const {
		return this->_blocks.end();
	};

	const uint8_t getColumn() const;
	const uint8_t getRow() const;
	const int8_t getFinalX(const TetroBlock&) const;
	const int8_t getFinalY(const TetroBlock&) const;
	sf::Color getColor() const;

	void rotate(rotation);
	void move(direction);

  protected:
	Tetro(sf::Color, BlockList);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

  private:
	uint8_t _col;
	uint8_t _row;
	sf::Color _c;
	BlockList _blocks;
};

class TetroFactory {
  public:
	typedef Tetro (* TetroCreator)();

	static Tetro createRandomTetro() {
		static TetroCreator creators[] = {
			&createI, &createJ, &createL, &createO, &createS, &createT, &createZ
		};
		const unsigned int numTetro = sizeof(creators)/sizeof(*creators);
		return creators[rand() % numTetro]();
	}

	static Tetro createI() {
		return Tetro(sf::Color::White, {
			{0,  1},
			{0,  0},
			{0, -1},
			{0, -2}
		});
	}

	static Tetro createJ() {
		return Tetro(sf::Color::Magenta, {
			{0,  1},
			{0,  0},
			{0, -1}, {-1, -1}
		});
	}

	static Tetro createL() {
		return Tetro(sf::Color::Yellow, {
			{0,  1},
			{0,  0},
			{0, -1}, {1, -1}
		});
	}

	static Tetro createO() {
		return Tetro(sf::Color::Red, {
			{0, 0}, {1, 0},
			{0, 1}, {1, 1}
		});
	}

	static Tetro createS() {
		return Tetro(sf::Color::Blue, {
			{1,  0}, { 0,  0},
			{0, -1}, {-1, -1}
		});
	}

	static Tetro createT() {
		return Tetro(sf::Color::Cyan, {
			{-1, 0}, {0, 0}, {1, 0},
			{ 0, 1}
		});
	}

	static Tetro createZ() {
		return Tetro(sf::Color::Green, {
			{-1,  0}, {0,  0},
			{ 0, -1}, {1, -1}
		});
	}
};

#endif /* TETRO_H */
