#include "logic.h"

void AbstractLogic::enqueue(std::shared_ptr<LogicCommand> lc) {
	_command_queue.push(std::move(lc));
}

void AbstractLogic::update() {
	while (!_command_queue.empty()) {
		std::shared_ptr<LogicCommand> c = _command_queue.front();
		c->perform();
		_command_queue.pop();
	}
}

Logic::Logic()
	: _current_tetro(_tetro_factory.next())
	, _command_factory(new BasicLogicCommandFactory(*this))
	, _gravity_task(&Logic::gravity, this), _has_gravity(true)
{}

Logic::~Logic() {
	this->_has_gravity = false;
	_gravity_task.join();
}

void Logic::record() {
	std::pair<uint8_t, uint8_t> minmax_row = _board.record(_current_tetro);
	std::map<uint8_t, Board> removed_boards = _board.deleteFullRows(
		minmax_row.first, minmax_row.second + 1);
	_current_tetro = _tetro_factory.next();
	this->callBack(LogicEvent::TetroAdded, removed_boards);
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
	Tetro t = _logic.getCurrentTetro();
	t.move(_dir);
	if (!_logic.getBoard().isOutOfSideBounds(t)) {
		if (_logic.getBoard().collides(t)) {
			if (_dir == DIR_DOWN) {
				_logic.record();
			}
		} else {
			_logic.getCurrentTetro() = t;
			_logic.callBack(LogicEvent::Move, _dir);
		}
	} else {
		_logic.callBack(LogicEvent::WallHit, t);
	}
}

void BasicRotateCommand::perform() {
	for (int i = 0; i < 5; i++) {
		Tetro t = _logic.getCurrentTetro();
		Tetro::WallKickTranslation trans = t.rotate(_rot, i);

		if (!_logic.getBoard().isOutOfSideBounds(t) && !_logic.getBoard().collides(t)) {
			Tetro::WallKickOffset off = t.getWallKickOffset(i);
			_logic.getCurrentTetro() = t;
			_logic.callBack(LogicEvent::Rotation, _rot, trans, off);
			return;
		}
	}
	_logic.callBack(LogicEvent::WallHit, _logic.getCurrentTetro());
}

void BasicDropCommand::perform() {
	Tetro cur = _logic.getCurrentTetro();
	Tetro& t = _logic.getCurrentTetro();
	uint8_t i = 0;
	while (!_logic.getBoard().collides(cur)) {
		t = cur;
		cur.move(DIR_DOWN);
		i++;
	}
	_logic.record();
	_logic.callBack(LogicEvent::Drop, (uint8_t) (i - 1));
}
