#include "drawing.h"

AnimatedDrawing::AnimatedDrawing(Logic& l, sf::RenderTarget& t)
	: _board(l._board), _current_tetro(l._current_tetro), _target(t)
{}

void AnimatedDrawing::update() {
	_target.draw(*this);
}

void AnimatedDrawing::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_board);
	target.draw(_current_tetro, states);
}

void AnimatedDrawing::onMoved(direction dir) {
}

void AnimatedDrawing::onRotated(rotation rot) {
}

void AnimatedDrawing::onWallHit(Tetro t) {
}
