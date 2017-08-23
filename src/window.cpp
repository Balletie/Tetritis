#include <map>
#include <thread>
#include "window.h"
#include "logic.h"
#include "ui_element.h"

namespace window {
	namespace {
		bool running = true;
		sf::RenderWindow window;
		Logic logic;
		Interface interface(window, logic);

		typedef std::map<sf::Keyboard::Key, std::shared_ptr<LogicCommand<Logic>>> CommandMap;

		CommandMap command_for_key = {
			{ sf::Keyboard::Left, logic._command_factory->createMoveCommand(DIR_LEFT) },
			{ sf::Keyboard::Right, logic._command_factory->createMoveCommand(DIR_RIGHT) },
			{ sf::Keyboard::Down, logic._command_factory->createMoveCommand(DIR_DOWN) },
			{ sf::Keyboard::Up, logic._command_factory->createDropCommand() },
			{ sf::Keyboard::Z, logic._command_factory->createRotateCommand(CCW) },
			{ sf::Keyboard::X, logic._command_factory->createRotateCommand(CW) }
		};

		bool handle_input() {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					return false;
				}

				if (event.type == sf::Event::Resized) {
					interface.resize(event.size.width, event.size.height);
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

			interface.update();

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
