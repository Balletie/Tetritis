#ifndef DRAWING_H
#define DRAWING_H
#include <chrono>
#include <SFML/Graphics.hpp>

#include "tweeny.h"
#include "macros.h"
#include "logic.h"

class AnimatedDrawing : public sf::Drawable {
  public:
	AnimatedDrawing(Logic&, sf::RenderTarget&);

	REGISTER_CALLBACK(onMoved, direction)
	REGISTER_CALLBACK(onRotated, rotation)
	REGISTER_CALLBACK(onWallHit, Tetro)
	REGISTER_CALLBACK(onDropped, uint8_t)

	void update();

  private:
	uint32_t restartClock();
	void onMoved(direction, uint8_t);

	std::chrono::steady_clock::time_point _start;

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

	Board& _board;
	Tetro& _current_tetro;
	sf::RenderTarget& _target;
	sf::Transform _transform;

	tweeny::tween<float, float> _tween;
};

#endif /* DRAWING_H */
