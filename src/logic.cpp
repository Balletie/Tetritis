#include "logic.h"

Logic::Logic()
	: _current_tetro(TetroFactory::createRandomTetro())
	, _command_factory(new BasicLogicCommandFactory(*this)) {}

void Logic::update() {
	while (!_command_queue.empty()) {
		std::shared_ptr<LogicCommand> c = _command_queue.front();
		c->perform();
		_command_queue.pop();
	}
}

void Logic::record() {
	_board.record(_current_tetro);
	_current_tetro = TetroFactory::createRandomTetro();
}

void Logic::recordOnCollision(Tetro& t) {
	if (_board.collides(_current_tetro)) {
		_current_tetro = t;
		record();
	} else {
		_current_tetro = t;
	}
}

void BasicMoveCommand::perform() {
	Tetro t = _logic._current_tetro;
	t.move(_dir);
	if (!_logic._board.isOutOfSideBounds(t)) {
		_logic.recordOnCollision(t);
		_onMovedCallback(_dir);
	} else {
		_onWallHitCallback(t);
	}
}

void BasicRotateCommand::perform() {
	Tetro t = _logic._current_tetro;
	t.rotate(_rot);
	if (!_logic._board.isOutOfSideBounds(t)) {
		_logic.recordOnCollision(t);
		_onRotatedCallback(_rot);
	} else {
		_onWallHitCallback(t);
	}
}

void BasicDropCommand::perform() {
	Tetro cur = _logic._current_tetro;
	Tetro& t = _logic._current_tetro;
	uint8_t i = 0;
	while (!_logic._board.collides(cur)) {
		t = cur;
		cur.move(DIR_DOWN);
		i++;
	}
	_logic.record();
	_onDroppedCallback(i - 1);
}

