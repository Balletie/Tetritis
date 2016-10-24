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

#define ENSURE_noWallHit(ID, CODE) \
	Tetro ID = _logic._current_tetro; \
	CODE; \
	if (!_logic._board.isOutOfSideBounds(ID)) { \
		_logic._current_tetro = ID; \
	}

void BasicMoveCommand::perform() {
	ENSURE_noWallHit(t, t.move(_dir));
}

void BasicRotateCommand::perform() {
	ENSURE_noWallHit(t, t.rotateLeft());
}

void BasicDropCommand::perform() {
	// Do nothing
}

