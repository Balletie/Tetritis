#ifndef TETRO_H
#define TETRO_H

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "block.h"
#include "movements.h"

class TetroFactory;

class Tetro : public sf::Drawable {
	public:

	typedef std::vector<TetroBlock> BlockList;
	typedef BlockList::const_iterator const_iterator;
	typedef std::pair<int8_t, int8_t> WallKickOffset;
	typedef std::pair<int8_t, int8_t> WallKickTranslation;
	typedef std::vector<WallKickOffset> WallKickOffsetRow;

	friend class TetroFactory;

	const const_iterator begin() const {
		return this->_blocks.begin();
	};

	const const_iterator end() const {
		return this->_blocks.end();
	};

	const uint8_t getColumn() const;
	const uint8_t getRow() const;
	const int8_t getFinalX(const TetroBlock&) const;
	const int8_t getFinalY(const TetroBlock&) const;
	sf::Color getColor() const;
	WallKickOffset getWallKickOffset(uint8_t);

	WallKickTranslation rotate(rotation, uint8_t = 0);
	void move(direction);

	static const sf::Color Cyan;
	static const sf::Color Yellow;
	static const sf::Color Purple;
	static const sf::Color Green;
	static const sf::Color Red;
	static const sf::Color Blue;
	static const sf::Color Orange;

	static const WallKickOffsetRow jlstz_offsets[4];
	static const WallKickOffsetRow i_offsets[4];
	static const WallKickOffsetRow o_offsets[4];

  protected:
	Tetro(sf::Color, BlockList, const WallKickOffsetRow (*)[4] = &jlstz_offsets);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

  private:
	void translate(uint8_t, uint8_t);
	WallKickTranslation getWallKickTranslation(rotation, uint8_t);

	uint8_t _col;
	uint8_t _row;
	orientation _orntn;
	const WallKickOffsetRow (*_wall_kick_offsets)[4];

	sf::Color _c;
	BlockList _blocks;
};

class TetroFactory {
  public:
	typedef Tetro (* TetroCreator)();

	static Tetro createRandomTetro() {
		static TetroCreator creators[] = {
			&createI, &createJ, &createL, &createO, &createS, &createT, &createZ
		};
		const unsigned int numTetro = sizeof(creators)/sizeof(*creators);
		return creators[rand() % numTetro]();
	}

	static Tetro createI() {
		return Tetro(Tetro::Cyan, {
			{-1, 0}, { 0, 0}, { 1, 0}, { 2, 0}
		}, &Tetro::i_offsets);
	}

	static Tetro createJ() {
		return Tetro(Tetro::Blue, {
			{-1,  1},
			{-1,  0}, {0, 0}, {1, 0}
		});
	}

	static Tetro createL() {
		return Tetro(Tetro::Orange, {
			                   {1, 1},
			{-1,  0}, {0,  0}, {1, 0}
		});
	}

	static Tetro createO() {
		return Tetro(Tetro::Yellow, {
			{0, 1}, {1, 1},
			{0, 0}, {1, 0}
		}, &Tetro::o_offsets);
	}

	static Tetro createS() {
		return Tetro(Tetro::Green, {
			         {0, 1}, {1,  1},
			{-1, 0}, {0, 0}
		});
	}

	static Tetro createT() {
		return Tetro(Tetro::Purple, {
			         {0, 1},
			{-1, 0}, {0, 0}, {1, 0}
		});
	}

	static Tetro createZ() {
		return Tetro(Tetro::Red, {
			{-1, 1}, {0, 1},
			         {0, 0}, {1, 0}
		});
	}
};

#endif /* TETRO_H */
