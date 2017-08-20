#include <map>
#include <thread>
#include "window.h"
#include "logic.h"
#include "drawing.h"

namespace window {
	namespace {
		bool running = true;
		sf::RenderWindow window;
		sf::View boardView(sf::FloatRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT));
		Logic logic;
		AnimatedDrawing drawing(logic, window);

		typedef std::map<sf::Keyboard::Key, std::shared_ptr<LogicCommand>> CommandMap;

		CommandMap command_for_key = {
			{ sf::Keyboard::Left, logic._command_factory->createMoveCommand(DIR_LEFT) },
			{ sf::Keyboard::Right, logic._command_factory->createMoveCommand(DIR_RIGHT) },
			{ sf::Keyboard::Down, logic._command_factory->createMoveCommand(DIR_DOWN) },
			{ sf::Keyboard::Up, logic._command_factory->createDropCommand() },
			{ sf::Keyboard::Z, logic._command_factory->createRotateCommand(CCW) },
			{ sf::Keyboard::X, logic._command_factory->createRotateCommand(CW) }
		};

		void fit_board_to_screen(float width, float height) {
			float boardRatio = (float) BOARD_WIDTH / (float) BOARD_HEIGHT;
			float screenRatio = width / height;
			float boardWidth = CELL_WIDTH_HEIGHT * BOARD_WIDTH;
			float boardHeight = CELL_WIDTH_HEIGHT * BOARD_HEIGHT;
			if (screenRatio > boardRatio) {
				width = boardWidth * height / boardHeight / width;
				height = 1;
			} else {
				height = boardHeight * width / boardWidth / height;
				width = 1;
			}
			sf::FloatRect viewPort = sf::FloatRect(0.5 - width / 2,
																						 0.5 - height / 2,
																						 width,
																						 height);
			boardView.setViewport(viewPort);
		}

		bool handle_input() {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					return false;
				}

				if (event.type == sf::Event::Resized) {
					fit_board_to_screen(event.size.width, event.size.height);
				}

				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							return false;
						case sf::Keyboard::G:
							logic.toggleGravity();
							break;
						default:
							auto entry = command_for_key.find(event.key.code);
							if (entry != command_for_key.end()) {
								logic.enqueue(entry->second);
							}
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
			logic.update();
			window.clear();
			window.setView(boardView);
			drawing.update();
			window.display();
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
		window.close();
		return true;
	}

	bool open() {
		window.create(
				sf::VideoMode(CELL_WIDTH_HEIGHT*BOARD_WIDTH,
					 CELL_WIDTH_HEIGHT*BOARD_HEIGHT),
				"Tetritis",
				sf::Style::Close | sf::Style::Titlebar);
		return true;
	}
}
