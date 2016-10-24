#ifndef DRAWING_H
#define DRAWING_H
#include <SFML/Graphics.hpp>

#include "macros.h"
#include "logic.h"

class AnimatedDrawing : public sf::Drawable {
  public:
	AnimatedDrawing(Logic&, sf::RenderTarget&);

	REGISTER_CALLBACK(onMoved, direction)
	REGISTER_CALLBACK(onRotated, rotation)
	REGISTER_CALLBACK(onWallHit, Tetro)

	void update();

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

	Board& _board;
	Tetro& _current_tetro;
	sf::RenderTarget& _target;
};

#endif /* DRAWING_H */
