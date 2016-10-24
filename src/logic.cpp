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

void BasicMoveCommand::perform() {
	Tetro t = _logic._current_tetro;
	t.move(_dir);
	if (!_logic._board.isOutOfSideBounds(t)) {
		_logic._current_tetro = t;
		_onMovedCallback(_dir);
	} else {
		_onWallHitCallback(t);
	}
}

void BasicRotateCommand::perform() {
	Tetro t = _logic._current_tetro;
	t.rotate(_rot);
	if (!_logic._board.isOutOfSideBounds(t)) {
		_logic._current_tetro = t;
		_onRotatedCallback(_rot);
	} else {
		_onWallHitCallback(t);
	}
}

void BasicDropCommand::perform() {
	// Do nothing
}

