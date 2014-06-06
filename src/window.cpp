#include "window.h"

namespace window {
	namespace {
		bool running = true;
		bool dirty   = true;
		sf::RenderWindow window;
		Board board;
		Tetro current_tetro(sf::Color::Red, TETRO_ZEE);

		bool handle_input() {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					return false;
				}

				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							return false;
						case sf::Keyboard::Return:
							current_tetro = Tetro::randomTetro();
							dirty = true;
							break;
						case sf::Keyboard::Left:
							current_tetro.move(DIR_LEFT);
							dirty = true;
							break;
						case sf::Keyboard::Right:
							current_tetro.move(DIR_RIGHT);
							dirty = true;
							break;
						case sf::Keyboard::Down:
							current_tetro.move(DIR_DOWN);
							dirty = true;
							break;
						case sf::Keyboard::Space:
							current_tetro.rotateLeft();
							dirty = true;
							break;
						default:
							break;
					}
				}
				if (event.type == sf::Event::MouseButtonPressed) {
					//Handle GUI stuff
				}
			}
			return true;
		}

		bool update() {
			// Save old state of block
			Tetro t = current_tetro;
			// TODO:Gravity
			if (!handle_input()) return false;
			if (board.collides(current_tetro)) {
				board.record(t);
				current_tetro = Tetro::randomTetro();
			}
			return true;
		}
	}

	bool loop() {
		while (running) {
			running = update();
			//if (board.collides(current_tetro)) current_tetro = Tetro::randomTetro();
			if (dirty) {
				window.clear();
				window.draw(board);
				window.draw(current_tetro);
				window.display();
				dirty = false;
			}
		}
		window.close();
		return true;
	}

	bool open() {
		window.create(	sf::VideoMode(	CELL_WIDTH_HEIGHT*BOARD_WIDTH,
						CELL_WIDTH_HEIGHT*BOARD_HEIGHT),
				"Tetritis",
				sf::Style::Close | sf::Style::Titlebar);
		return true;
	}
}
