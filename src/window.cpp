#include "window.h"
#include <iostream>

namespace window {
	namespace {
		bool running = true;
		bool dirty   = true;
		sf::RenderWindow window;
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

	}

	bool loop() {
		while (running) {
			running = handle_input();
	//			if (current_tetro.collides(&board)) current_tetro = Tetro::randomTetro();
			if (dirty) {
				window.clear();
	//			window.draw(board);
				window.draw(current_tetro);
				window.display();
				dirty = false;
			}
		}
		window.close();
		return true;
	}

	bool open() {
		window.create(sf::VideoMode(300, 700), "Tetritis", sf::Style::Close | sf::Style::Titlebar);
		return true;
	}
}
