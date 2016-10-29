#ifndef DRAWING_H
#define DRAWING_H
#include <chrono>
#include <SFML/Graphics.hpp>

#include "macros.h"
#include "animation.h"

class AnimatedDrawing {
  public:
	AnimatedDrawing(Logic&, sf::RenderTarget&);

	void update();

  private:
	uint32_t restartClock();

	std::chrono::steady_clock::time_point _start;

  protected:
	std::vector<std::unique_ptr<AnimatedDrawable>> _drawables;
	sf::RenderTarget& _target;
};

#endif /* DRAWING_H */
