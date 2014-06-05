#include "tetro.h"
#include <map>

const uint8_t BOARD_WIDTH = 10;
const uint8_t BOARD_HEIGHT = 22;

class Board : public sf::Drawable {
  public:
	Board();
	bool collide(Tetro);
  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;	
  private:
	std::map<uint8_t, tet_type> _data;
};
