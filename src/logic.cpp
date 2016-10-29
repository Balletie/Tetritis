#include "logic.h"

Logic::Logic()
	: _current_tetro(TetroFactory::createRandomTetro())
	, _command_factory(new BasicLogicCommandFactory(*this))
	, _gravity_task(&Logic::gravity, this), _has_gravity(true)
{}

Logic::~Logic() {
	this->_has_gravity = false;
	_gravity_task.join();
}

void Logic::update() {
	while (!_command_queue.empty()) {
		std::shared_ptr<LogicCommand> c = _command_queue.front();
		c->perform();
		_command_queue.pop();
	}
}

std::map<uint8_t, Board> Logic::record() {
	std::pair<uint8_t, uint8_t> minmax_row = _board.record(_current_tetro);
	std::map<uint8_t, Board> removed_boards = _board.deleteFullRows(
		minmax_row.first, minmax_row.second + 1);
	_current_tetro = TetroFactory::createRandomTetro();
	return removed_boards;
}

void Logic::gravity() {
	std::shared_ptr<LogicCommand> moveDownCommand =
		this->_command_factory->createMoveCommand(DIR_DOWN);
	while (_has_gravity) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		this->enqueue(moveDownCommand);
	}
}

void BasicMoveCommand::perform() {
	Tetro t = _logic._current_tetro;
	t.move(_dir);
	if (!_logic._board.isOutOfSideBounds(t)) {
		if (_logic._board.collides(t)) {
			if (_dir == DIR_DOWN) {
				std::map<uint8_t, Board> removed_boards = _logic.record();
				_logic.callBack(LogicEvent::TetroAdded, removed_boards);
			}
		} else {
			_logic._current_tetro = t;
			_logic.callBack(LogicEvent::Move, _dir);
		}
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
	std::map<uint8_t, Board> removed_boards = _logic.record();
	_logic.callBack(LogicEvent::Drop, (uint8_t) (i - 1));
	_logic.callBack(LogicEvent::TetroAdded, removed_boards);
}

