#include "ghost_tetro.h"

GhostTetro::GhostTetro(AbstractLogic& l)
	: _ghost(l.getCurrentTetro())
	, _actual(l.getCurrentTetro())
	, _gl(_ghost, l.getBoard())
	, _drop_command(BasicLogicCommandFactory(_gl).createDropCommand())
{
	updateGhost();
	l.addCallback(LogicEvent::Move, this->onMoved_cb());
	l.addCallback(LogicEvent::Rotation, this->onRotated_cb());
	l.addCallback(LogicEvent::Drop, this->onDropped_cb());
}

void GhostTetro::updateGhost() {
	_ghost = _actual;
	_ghost._c.a = 120;
	_gl.enqueue(_drop_command);
}

void GhostTetro::step(uint32_t dt) {
	_gl.update();
}

void GhostTetro::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_ghost, states);
}

void GhostTetro::onMoved(direction dir) {
	updateGhost();
}

void GhostTetro::onRotated(rotation rot, Tetro::WallKickTranslation trans, Tetro::WallKickOffset off) {
	updateGhost();
}

void GhostTetro::onDropped(uint8_t height) {
	updateGhost();
}
