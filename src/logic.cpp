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
	if (_board.collides(t)) {
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
		_logic.callBack(LogicEvent::Move, _dir);
	} else {
		_logic.callBack(LogicEvent::WallHit, t);
	}
}

void BasicRotateCommand::perform() {
	for (int i = 0; i < 5; i++) {
		Tetro t = _logic._current_tetro;
		Tetro::WallKickTranslation trans = t.rotate(_rot, i);

		if (!_logic._board.isOutOfSideBounds(t) && !_logic._board.collides(t)) {
			Tetro::WallKickOffset off = t.getWallKickOffset(i);
			_logic._current_tetro = t;
			_logic.callBack(LogicEvent::Rotation, _rot, trans, off);
			return;
		}
	}
	_logic.callBack(LogicEvent::WallHit, _logic._current_tetro);
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
	_logic.callBack(LogicEvent::Drop, i - 1);
}

