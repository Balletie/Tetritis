#include <SFML/Graphics/VertexArray.hpp>
#include "board.h"
#include <iostream>

Board::Board() 
{}

bool Board::collides(Tetro& t) {
	const Block* bs = t.getData();
	for (int i = 0; i < 4; i++) {
		const int8_t fin_y = t.getFinalY(bs + i);
		if (fin_y < 1 || fin_y >= BOARD_HEIGHT)	return true;

		const int8_t fin_x = t.getFinalX(bs + i);
		if (fin_x < 0 || fin_x >= BOARD_WIDTH)	return true;

		// coordinates are within board bounds, so we can construct a key.
		uint16_t key = fin_y * BOARD_WIDTH + fin_x;
		if (_data.find(key) == _data.end()) 	continue;
		else					return true;
	}
	return false;
}

void Board::record(Tetro& t) {
	const Block* bs = t.getData();
	for (int i = 0; i < 4; i++) {
		const int8_t fin_y = t.getFinalY(bs + i);
		if (fin_y < 1 || fin_y > BOARD_HEIGHT)	return;

		const int8_t fin_x = t.getFinalX(bs + i);
		if (fin_x < 0 || fin_x > BOARD_WIDTH)	return;
		std::cout<<'('<<(int)fin_x<<','<<(int)fin_y<<")\n";
		_data[(uint16_t)fin_y * BOARD_WIDTH + fin_x] = bs[i].getColor();
	}
	std::cout<<std::endl;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if(_data.empty()) return;

	sf::VertexArray vertices(sf::Quads, 4*_data.size());
	typename std::map<uint16_t, sf::Color>::const_iterator it = _data.begin();

	for (int i = 0; it != _data.end(); ++it, i++) {
		sf::Vertex* quad = &vertices[4*i];
		uint16_t key  = it->first;
		sf::Color cl = it->second;
		uint16_t x = CELL_WIDTH_HEIGHT * (key % BOARD_WIDTH);
		uint16_t y = CELL_WIDTH_HEIGHT * (key / BOARD_WIDTH);

		quad[0].position = sf::Vector2f(x, y);
		quad[1].position = sf::Vector2f(x + CELL_WIDTH_HEIGHT, y);
		quad[2].position = sf::Vector2f(x + CELL_WIDTH_HEIGHT, y + CELL_WIDTH_HEIGHT);
		quad[3].position = sf::Vector2f(x, y + CELL_WIDTH_HEIGHT);
		for (int j = 0; j < 4; j++) quad[j].color = cl;
	}
	target.draw(vertices);
}
