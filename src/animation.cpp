#include "animation.h"
#include "matrices.h"

void AnimatedTetro::onMoved(direction dir, uint8_t height) {
	_tween =
		tweeny::from(
			(float) (dir * CELL_WIDTH_HEIGHT / 2),
			(float) (height * (dir - 1) % 2 * CELL_WIDTH_HEIGHT / 2))
		.to(0.f,0.f)
		.during(50)
		.onStep([this](float x, float y) { _transform = translation_mat(x, y); return false; });
}

void AnimatedTetro::onMoved(direction dir) {
	this->onMoved(dir, 1);
}

void AnimatedTetro::onRotated(rotation rot, Tetro::WallKickTranslation trans, Tetro::WallKickOffset off) {
	float trans_x = (rot * trans.second - trans.first)  * 0.5;
	float trans_y = (rot * trans.first  + trans.second) * 0.5;
	float centerX = ((float)_drawable.getColumn() + trans_x) * CELL_WIDTH_HEIGHT + CELL_WIDTH_HEIGHT / 2;
	float centerY = ((float)_drawable.getRow() + trans_y) * CELL_WIDTH_HEIGHT + CELL_WIDTH_HEIGHT / 2;
	_tween =
		tweeny::from((float) -1 * rot * 90.f, 0.f)
		.to(0.f, 0.f)
		.during(50)
		.onStep([this,centerX,centerY,off](float t, float) {
			_transform = translation_mat(-off.first, -off.second).rotate(t, centerX, centerY);
			return false;
		});
}

void AnimatedTetro::onDropped(uint8_t height) {
	this->onMoved(DIR_DOWN, height);
	_tween.via(tweeny::easing::bounceOut);
}

void AnimatedBoard::step(uint32_t dt) {
	std::for_each(_tweens.begin(), _tweens.end(), [dt](tweeny::tween<float> t) {
		t.step(dt);
	});
}

void AnimatedBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto it = _remaining_boards.begin(); it != _remaining_boards.end(); it++) {
		states.transform = translation_mat(0, -1 * it->first * CELL_WIDTH_HEIGHT);
		target.draw(it->second, states);
	}
}

std::map<uint8_t, Board> AnimatedBoard::constructRemainingBoards() {
	std::map<uint8_t, Board> remaining;
	uint8_t i = 0;
	uint8_t accum_height = 0;
	for (auto it = _removed_boards.begin(); it != _removed_boards.end(); it++) {
		printf("rm rows(%u, %zu)\n", it->first, it->first + it->second.height());
		if (it->first == i) {
			accum_height += it->second.height();
			continue;
		}
		uint8_t end = it->first - accum_height;
		printf("%u -> rows(%u, %u)\n", i + accum_height, i, end);
		Board&& remain = _board.subBoard(_board.begin() + i, _board.begin() + end);
		remaining.emplace(i + accum_height, remain);
		i = it->first - accum_height;
		accum_height += it->second.height();
	}
	if (_board.begin() + i != _board.end()) {
		printf("%u -> rows(%u, end)\n", i + accum_height, i);
		remaining.emplace(i + accum_height, _board.subBoard(_board.begin() + i, _board.end()));
	}
	return remaining;
}

void AnimatedBoard::onTetroAdded(std::map<uint8_t, Board> bs) {
	_removed_boards = bs;
	_remaining_boards = constructRemainingBoards();
}
