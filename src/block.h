#ifndef BLOCK_H
#define BLOCK_H
#include <stdint.h>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Vertex.hpp>

void drawVertices(sf::Vertex*, const sf::Vector2f&);

class Tetro;
class Board;

const uint8_t CELL_WIDTH_HEIGHT = 16;

struct TetroBlock {
  public:
	TetroBlock(int8_t, int8_t);

  protected:
	friend class Tetro;

	/*
	 * Function for drawing to the given vertex array with the given transform,
	 * which can be in board coordinates or tetrominos coordinates.
	 */
	void drawVertices(sf::Vertex*, const sf::Color&) const;

	// The x and y coordinates are relative to the center of a tetrominos.
	int8_t _x;
	int8_t _y;
};

struct BoardBlock {
	BoardBlock(sf::Color);
	sf::Color getColor() const;

  protected:
	friend class Board;

	void drawVertices(sf::Vertex*, uint16_t, uint16_t) const;

	sf::Color _c;
};

#endif /* BLOCK_H */
