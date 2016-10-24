#include "block.h"

void drawVertices(sf::Vertex *quad, const sf::Vector2f& pos,
		const sf::Color& c) {
	quad[0].position = pos;
	quad[1].position = pos + sf::Vector2f(CELL_WIDTH_HEIGHT, 0);
	quad[2].position = pos + sf::Vector2f(CELL_WIDTH_HEIGHT, CELL_WIDTH_HEIGHT);
	quad[3].position = pos + sf::Vector2f(0, CELL_WIDTH_HEIGHT);
	for (int j = 0; j < 4; j++) quad[j].color = c;
}

TetroBlock::TetroBlock(int8_t x, int8_t y) : _x(x), _y(y) {}

void TetroBlock::drawVertices(sf::Vertex *quad, const sf::Transform& transform,
		const sf::Color& c) const {
	sf::Vector2f final_x_y = transform.transformPoint((float)_x, -(float)_y);
	::drawVertices(quad, final_x_y, c);
}

BoardBlock::BoardBlock(sf::Color c) : _c(c) {}

sf::Color BoardBlock::getColor() const {
	return this->_c;
}

void BoardBlock::drawVertices(sf::Vertex* quad, uint16_t x, uint16_t y) const {
	::drawVertices(quad, sf::Vector2f((float) x, (float) y), this->getColor());
}
