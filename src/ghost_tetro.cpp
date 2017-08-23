#include "ghost_tetro.h"

GhostTetro::GhostTetro(Tetro& t, Board& b)
	: _actual(t)
	, _ghost(_actual)
	, _gl(_ghost, b)
	, _drop_command(BasicLogicCommandFactory<GhostLogic>(_gl).createDropCommand())
{
	updateGhost();
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
