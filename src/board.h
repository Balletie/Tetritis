#include "tetro.h"
#include <map>

const uint8_t BOARD_WIDTH = 10;
const uint8_t BOARD_HEIGHT = 22;

class Board : public sf::Drawable {
  public:
	Board();
	bool collides(Tetro&);
	void record(Tetro&);
  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;	
  private:
	void delete_row(uint8_t);
	uint8_t _row_sizes[BOARD_HEIGHT];
	std::map<uint8_t, sf::Color> _data;
};
