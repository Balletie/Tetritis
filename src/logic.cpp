#include "logic.h"

Logic::Logic()
	: _current_tetro(Tetro::randomTetro())
	, _command_factory(new BasicLogicCommandFactory(*this)) {}

void Logic::update() {
	Tetro t = _current_tetro;
	this->emptyCommandQueue();

	if (_board.collides(_current_tetro)) {
		_board.record(t);
		_current_tetro = Tetro::randomTetro();
	}
}

void Logic::emptyCommandQueue() {
	while (!_command_queue.empty()) {
		std::shared_ptr<LogicCommand> c = _command_queue.front();
		c->perform();
		_command_queue.pop();
	}
}

void Logic::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_board);
	target.draw(_current_tetro);
}

void BasicMoveCommand::perform() {
	Tetro t = _logic._current_tetro;
	t.move(_dir);
	if (!_logic._board.isOutOfSideBounds(t)) {
		_logic._current_tetro = t;
	}
}

void BasicRotateCommand::perform() {
	_logic._current_tetro.rotateLeft();
}

void BasicDropCommand::perform() {
	// Do nothing
}

