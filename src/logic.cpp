#include "logic.h"

Logic::Logic()
	: _command_factory(new BasicLogicCommandFactory<Logic>(*this))
	, _gravity_task(&Logic::gravity, this), _has_gravity(false)
	, _tetro_factory(new GuidelineTetroFactory())
{
	this->spawnNext();
}

Logic::~Logic() {
	this->_has_gravity = false;
	_gravity_task.join();
}

void Logic::record() {
	std::pair<uint8_t, uint8_t> minmax_row = _board.record(_tetro);
	std::map<uint8_t, Board> removed_boards = _board.deleteFullRows(
		minmax_row.first, minmax_row.second + 1);
	this->spawnNext();
	this->callBack(LogicEvent::TetroAdded, removed_boards);
}

void Logic::spawnNext() {
	_tetro = _tetro_factory->next();
	_tetro.translate(0, -2);
	for (int i = 0; _board.collides(_tetro) && i < 2; i++) {
		_tetro.translate(0, 1);
	}
}

void Logic::gravity() {
	std::shared_ptr<LogicCommand<Logic>> moveDownCommand =
		this->_command_factory->createMoveCommand(DIR_DOWN);
	while (_has_gravity) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		this->enqueue(moveDownCommand);
	}
}

