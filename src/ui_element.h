#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H
#include <chrono>
#include <SFML/Graphics.hpp>

#include "board.h"
#include "macros.h"
#include "animation.h"
#include "matrices.h"

const uint8_t CELL_WIDTH_HEIGHT = 16;

class Interface;

class InterfaceElement {
	friend Interface;

  public:
	InterfaceElement() {}
	virtual void update(sf::RenderTarget&, sf::RenderStates);

	virtual ~InterfaceElement() {}

  protected:
	// Target width and height in pixels.
	virtual float width() = 0;
	virtual float height() = 0;
	virtual float frameWidth() {
		return 1;
	}
	
	virtual float totalWidth() {
		return width() + frameWidth() * 2;
	}
	
	virtual float totalHeight() {
		return height() + frameWidth() * 2;
	}

  protected:
	sf::VertexArray createFrame();

	sf::VertexArray _frame_vertices;
};

class AnimatedPlayfield : public InterfaceElement {
  public:
	AnimatedPlayfield(Logic&);
	
	void update(sf::RenderTarget&, sf::RenderStates) override;

	float width() override {
		return BOARD_WIDTH;
	}

	float height() override {
		return BOARD_HEIGHT - 2;
	}
	
	float frameWidth() override {
		return 0.5;
	}

  private:
	uint32_t restartClock();

	std::chrono::steady_clock::time_point _start;

  protected:
	std::vector<std::unique_ptr<AnimatedDrawable>> _drawables;
};

class Interface {
  public:
	Interface(sf::RenderTarget& target, Logic& l)
		: _target(target)
		, _states(sf::RenderStates::Default)
	{
		_elems.emplace_back(sf::Vector2u(0, 2), std::make_unique<AnimatedPlayfield>(l));
		this->resetView();
	}

	void update();
	void resize(float, float);

  protected:
	// Target width and height in pixels.
	float width();
	float height();
	float coordWidth();
	float coordHeight();

  private:
  void keepAspectRatio(float, float);
	void resetView() {
		_view.reset(sf::FloatRect(0, 0, coordWidth(), coordHeight()));
	}
	void setView(float width, float height) {
		sf::FloatRect newView(- (width / CELL_WIDTH_HEIGHT / 2) + coordWidth() / 2, 0, width / CELL_WIDTH_HEIGHT, height / CELL_WIDTH_HEIGHT);
		_view.reset(newView);
	}

  protected:
	sf::RenderTarget& _target;
	sf::RenderStates _states;
	sf::View _view;
	std::vector<std::pair<sf::Vector2u, std::unique_ptr<InterfaceElement>>> _elems;
};

#endif /* UI_ELEMENT_H */
