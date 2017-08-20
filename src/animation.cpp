#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include "animation.h"
#include "matrices.h"

void AnimatedTetro::onMoved(direction dir, uint8_t height) {
	_tween =
		tweeny::from(
			(float) dir,
			(float) height * fmodf(dir - 1, 2.f))
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
	float centerX = ((float)_drawable.getColumn() + trans_x) + 0.5;
	float centerY = ((float)_drawable.getRow() + trans_y) + 0.5;
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
	for (auto it = _tweens.begin(); it < _tweens.end(); it++) {
		it->step(dt);
	}
	_opacity_tween.step(dt);
}

void AnimatedBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	drawRemoved(target, states);
	drawRemaining(target, states);
}

void AnimatedBoard::drawRemoved(sf::RenderTarget& target, sf::RenderStates states) const {
	if (_opacity_tween.peek() < 0)
		return;
	std::map<uint8_t, Board>::const_iterator rbs = _removed_boards.begin();
	for (; rbs != _removed_boards.end(); rbs++) {
		states.blendMode = sf::BlendAlpha;
		sf::RectangleShape alpha(sf::Vector2f((float) BOARD_WIDTH,
			(float) rbs->second.height()));
		sf::Color c(255, 255, 255, _opacity_tween.peek());
		alpha.setFillColor(c);
		alpha.setPosition(0.f, BOARD_HEIGHT - rbs->second.height() - rbs->first);
		states.transform = sf::Transform();
		target.draw(alpha, states);

		// This multiplies (r, g, b, a) * (1, 1, 1, tween_alpha), effectively setting
		// the alpha channel for the pixels of the removed board.
		states.blendMode = sf::BlendMultiply;
		states.transform = translation_mat(0, -1 * rbs->first);
		target.draw(rbs->second, states);
	}
}

void AnimatedBoard::drawRemaining(sf::RenderTarget& target, sf::RenderStates states) const {
	states.blendMode = sf::BlendAlpha;
	std::map<uint8_t, Board>::const_iterator bs = _remaining_boards.begin();
	std::vector<tweeny::tween<float>>::const_iterator ts = _tweens.begin();
	for (; ts != _tweens.end() && bs != _remaining_boards.end(); ++ts, bs++) {
		states.transform = translation_mat(0, -1 * bs->first + ts->peek());
		target.draw(bs->second, states);
	}
}

void AnimatedBoard::addTweenForHeight(uint8_t height) {
	tweeny::tween<float> tw = tweeny::from(0.f)
		.to((float) height)
		.during(400)
		.via(tweeny::easing::bounceOut);
	_tweens.push_back(tw);
}

std::map<uint8_t, Board> AnimatedBoard::constructRemainingBoards() {
	std::map<uint8_t, Board> remaining;
	uint8_t i = 0;
	uint8_t accum_height = 0;
	for (auto it = _removed_boards.begin(); it != _removed_boards.end(); it++) {
		if (it->first == i) {
			accum_height += it->second.height();
			continue;
		}
		uint8_t end = it->first - accum_height;
		Board&& remain = _board.subBoard(_board.begin() + i, _board.begin() + end);
		remaining.emplace(i + accum_height, remain);
		addTweenForHeight(accum_height);
		i = it->first - accum_height;
		accum_height += it->second.height();
	}
	if (_board.begin() + i != _board.end()) {
		remaining.emplace(i + accum_height, _board.subBoard(_board.begin() + i, _board.end()));
		addTweenForHeight(accum_height);
	}
	return remaining;
}

void AnimatedBoard::onTetroAdded(std::map<uint8_t, Board> bs) {
	_tweens.clear();
	_removed_boards = bs;
	_remaining_boards = constructRemainingBoards();
	_opacity_tween.seek(0.f);
}
