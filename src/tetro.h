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
	const float getCenterX() const;
	const float getCenterY() const;
	const int8_t getFinalX(const TetroBlock&) const;
	const int8_t getFinalY(const TetroBlock&) const;
	sf::Color getColor() const;

	void rotate(rotation);
	void move(direction);

	static const sf::Color Cyan;
	static const sf::Color Yellow;
	static const sf::Color Purple;
	static const sf::Color Green;
	static const sf::Color Red;
	static const sf::Color Blue;
	static const sf::Color Orange;

  protected:
	Tetro(sf::Color, float, float, BlockList);
	Tetro(sf::Color, BlockList);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

  private:
	uint8_t _col;
	uint8_t _row;
	float _center_x;
	float _center_y;
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
		return Tetro(Tetro::Cyan, 0.5, -0.5, {
			{-1, 0}, { 0, 0}, { 1, 0}, { 2, 0}
		});
	}

	static Tetro createJ() {
		return Tetro(Tetro::Blue, {
			{-1,  1},
			{-1,  0}, {0, 0}, {1, 0}
		});
	}

	static Tetro createL() {
		return Tetro(Tetro::Orange, {
			{-1,  0}, {0,  0}, {1, 0},
			{-1, -1}
		});
	}

	static Tetro createO() {
		return Tetro(Tetro::Yellow, 0.5, 0.5, {
			{0, 0}, {1, 0},
			{0, 1}, {1, 1}
		});
	}

	static Tetro createS() {
		return Tetro(Tetro::Green, {
			         {0, 1}, {1,  1},
			{-1, 0}, {0, 0}
		});
	}

	static Tetro createT() {
		return Tetro(Tetro::Purple, {
			         {0, 1},
			{-1, 0}, {0, 0}, {1, 0}
		});
	}

	static Tetro createZ() {
		return Tetro(Tetro::Red, {
			{-1, 1}, {0, 1},
			         {0, 0}, {1, 0}
		});
	}
};

#endif /* TETRO_H */
