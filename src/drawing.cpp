#include "drawing.h"
#include "matrices.h"
#include "tweeny.h"

AnimatedDrawing::AnimatedDrawing(Logic& l, sf::RenderTarget& t)
	: _start(std::chrono::steady_clock::now())
	, _board(l._board), _current_tetro(l._current_tetro), _target(t)
	, _tween(tweeny::from(0.f,0.f).to(0.f,0.f).during(1))
{}

void AnimatedDrawing::update() {
	_tween.step(restartClock());
	_target.draw(*this);
}

void AnimatedDrawing::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_board);
	states.transform = this->_transform;
	target.draw(_current_tetro, states);
}

uint32_t AnimatedDrawing::restartClock() {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::milliseconds dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start);
	_start = end;
	return dt.count();
}

void AnimatedDrawing::onMoved(direction dir, uint8_t height) {
	_tween =
		tweeny::from(
			(float) (dir * CELL_WIDTH_HEIGHT / 2),
			(float) (height * (dir - 1) % 2 * CELL_WIDTH_HEIGHT / 2))
		.to(0.f,0.f)
		.during(50)
		.onStep([this](float x, float y) { _transform = translation_mat(x, y); return false; });
}

void AnimatedDrawing::onMoved(direction dir) {
	this->onMoved(dir, 1);
}

void AnimatedDrawing::onRotated(rotation rot) {
	float centerX = _current_tetro.getColumn() * CELL_WIDTH_HEIGHT + CELL_WIDTH_HEIGHT / 2;
	float centerY = _current_tetro.getRow() * CELL_WIDTH_HEIGHT + CELL_WIDTH_HEIGHT / 2;
	_tween =
		tweeny::from((float) rot * 90.f, 0.f)
		.to(0.f, 0.f)
		.during(50)
		.onStep([this,centerX,centerY](float t, float) {
			_transform = rotation_mat(t, centerX, centerY);
			return false;
		});
}

void AnimatedDrawing::onWallHit(Tetro t) {
}

void AnimatedDrawing::onDropped(uint8_t height) {
	this->onMoved(DIR_DOWN, height);
}
