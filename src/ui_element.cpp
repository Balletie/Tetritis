#include <iostream>
#include <numeric>
#include <functional>
#include "matrices.h"
#include "ui_element.h"
#include "ghost_tetro.h"

using namespace std::placeholders;

float Interface::width() {
	return CELL_WIDTH_HEIGHT * coordWidth();
}

float Interface::height() {
	return CELL_WIDTH_HEIGHT * coordHeight();
}

float Interface::coordWidth() {
	std::vector<float> widths;
	std::transform(_elems.begin(), _elems.end(), std::back_inserter(widths),
								 [](auto& e) {
									 return e.first.x + e.second->totalWidth(); });
	return *(std::max_element(widths.begin(), widths.end()));
}

float Interface::coordHeight() {
	std::vector<float> heights;
	std::transform(_elems.begin(), _elems.end(), std::back_inserter(heights),
								 [](auto& e) {
									 return e.first.y + e.second->totalHeight(); });
	return *(std::max_element(heights.begin(), heights.end()));
}

void Interface::update() {
	_target.setView(_view);
	std::for_each(_elems.begin(), _elems.end(), [&](auto &locToElem) {
		sf::Vector2i l = locToElem.first;
		locToElem.second->update(_target, translation_mat(l.x, l.y));
	});
}

void Interface::resize(float width, float height) {
	if (this->width() > width || this->height() > height) {
		// Screen is getting too small: resize while maintaining aspect ratio.
		keepAspectRatio(width, height);
	} else {
		// Resize view according to new width and height.
		setView(width, height);
		_view.setViewport(_target.getDefaultView().getViewport());
	}
}

void Interface::keepAspectRatio(float screenWidth, float screenHeight) {
	float width = this->width();
	float height = this->height();
	float aspectRatio = width / height;
	float screenRatio = screenWidth / screenHeight;

	if (screenRatio > aspectRatio) {
		width = width * screenHeight / height / screenWidth;
		height = 1;
	} else {
		height = height * screenWidth / width / screenHeight;
		width = 1;
	}

	// Centered in the middle.
	sf::FloatRect viewPort = sf::FloatRect(0.5 - width / 2, 0, width, height);
	resetView();
	_view.setViewport(viewPort);
}

void InterfaceElement::update(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(_frame_vertices, states);
}

// ASCII drawing:
// 0,8----------- 2
//  |\\   __--``/ |
//  |  1,9--- 3   |
//  | \ |     |\  |
//  |   |     |   |
//  |  \|     | \ |
//  |   7 --- 5.  |
//  | / __--``  `\|
//  6 ----------- 4
sf::VertexArray InterfaceElement::createFrame() {
	sf::VertexArray vertices(sf::TriangleStrip, 10);
	vertices[0].position  = sf::Vector2f(0, 0);
	vertices[1].position  = sf::Vector2f(frameWidth(), frameWidth());
	vertices[2].position  = sf::Vector2f(this->width() + 2 * frameWidth(), 0);
	vertices[3].position  = sf::Vector2f(this->width() + frameWidth(), frameWidth());
	vertices[4].position  = sf::Vector2f(this->width() + 2 * frameWidth(), this->height() + 2 * frameWidth());
	vertices[5].position  = sf::Vector2f(this->width() + frameWidth(), this->height() + frameWidth());
	vertices[6].position  = sf::Vector2f(0, this->height() + 2 * frameWidth());
	vertices[7].position  = sf::Vector2f(frameWidth(), this->height() + frameWidth());
	vertices[8].position  = sf::Vector2f(0, 0);
	vertices[9].position  = sf::Vector2f(frameWidth(), frameWidth());

	vertices[0].color = sf::Color::Red;
	vertices[1].color = sf::Color::Red;
	vertices[2].color = sf::Color::Blue;
	vertices[3].color = sf::Color::Blue;
	vertices[4].color = sf::Color::Blue;
	vertices[5].color = sf::Color::Blue;
	vertices[6].color = sf::Color::Red;
	vertices[7].color = sf::Color::Red;
	vertices[8].color = sf::Color::Red;
	vertices[9].color = sf::Color::Red;

	return vertices;
}

AnimatedPlayfield::AnimatedPlayfield(Logic& l)
	: _start(std::chrono::steady_clock::now())
{
	AnimatedTetro *t = new AnimatedTetro(l.getCurrentTetro());
	l.addCallback(LogicEvent::Move, t->onMoved_cb());
	l.addCallback(LogicEvent::Rotation, t->onRotated_cb());
	l.addCallback(LogicEvent::Drop, t->onDropped_cb());

	AnimatedBoard *b = new AnimatedBoard(l.getBoard());
	l.addCallback(LogicEvent::TetroAdded, b->onTetroAdded_cb());

	GhostTetro *gt = GhostTetro::make_ghost(l);

	_drawables.emplace_back(t);
	_drawables.emplace_back(b);
	_drawables.emplace_back(gt);

	_frame_vertices = createFrame();
}

void AnimatedPlayfield::update(sf::RenderTarget& target, sf::RenderStates states) {
	InterfaceElement::update(target, states);
	states.transform.translate(this->frameWidth(), this->frameWidth() - 2);

	uint32_t dt = restartClock();
	std::for_each(_drawables.begin(), _drawables.end(), [&](auto &d) {
		d->step(dt);
		target.draw(*d, states);
	});
}

uint32_t AnimatedPlayfield::restartClock() {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::milliseconds dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start);
	_start = end;
	return dt.count();
}

TetroPreview::TetroPreview(Logic& l)
	: _tetro_factory(l.getFactory())
{
	_frame_vertices = createFrame();
}

void TetroPreview::update(sf::RenderTarget& target, sf::RenderStates states) {
	InterfaceElement::update(target, states);
	states.transform.translate(this->frameWidth(), this->frameWidth());
	
	std::for_each(_tetro_factory.begin(), _tetro_factory.end(), [&](auto t) {
		states.transform.translate(5, 0);
		target.draw(t, states);
	});
}
