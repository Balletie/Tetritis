#include <algorithm>
#include "drawing.h"
#include "matrices.h"
#include "tweeny.h"
#include "ghost_tetro.h"

AnimatedDrawing::AnimatedDrawing(Logic& l, sf::RenderTarget& trgt)
	: _start(std::chrono::steady_clock::now()), _target(trgt)
{
	AnimatedTetro *t = new AnimatedTetro(l._current_tetro);
	l.addCallback(LogicEvent::Move, t->onMoved_cb());
	l.addCallback(LogicEvent::Rotation, t->onRotated_cb());
	l.addCallback(LogicEvent::Drop, t->onDropped_cb());

	AnimatedBoard *b = new AnimatedBoard(l._board);
	l.addCallback(LogicEvent::TetroAdded, b->onTetroAdded_cb());

	GhostTetro *gt = new GhostTetro(l);

	_drawables.emplace_back(t);
	_drawables.emplace_back(b);
	_drawables.emplace_back(gt);
}

void AnimatedDrawing::update() {
	uint32_t dt = restartClock();
	std::for_each(_drawables.begin(), _drawables.end(), [dt,this](auto &d) {
		d->step(dt);
		_target.draw(*d);
	});
}

uint32_t AnimatedDrawing::restartClock() {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::milliseconds dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start);
	_start = end;
	return dt.count();
}
