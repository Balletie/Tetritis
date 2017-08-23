#ifndef GHOST_TETRO_H
#define GHOST_TETRO_H

#include "animation.h"
#include "tetro.h"

class GhostLogic : public AbstractLogic {
  public:
	GhostLogic(Tetro& ghost, const Board& board)
		: _board(board), _ghost(ghost) {
	}

	Tetro& currentTetro() const override {
		return _ghost;
	}

	Tetro getCurrentTetro() const override {
		return _ghost;
	}

	const Board& getBoard() const override {
		return _board;
	}

	void record() override {
		// Do nothing.
	}

	const Board& _board;
	Tetro& _ghost;
};

class GhostTetro : public AnimatedDrawable {
  public:
	GhostTetro(AbstractLogic&);

	REGISTER_CALLBACK(onMoved, direction)
	REGISTER_CALLBACK(onRotated, rotation, Tetro::WallKickTranslation,
		Tetro::WallKickOffset)
	REGISTER_CALLBACK(onDropped, uint8_t)

  protected:
	void step(uint32_t dt) override;

	void updateGhost();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
	std::shared_ptr<LogicCommand> _drop_command;
	GhostLogic _gl;
	Tetro _ghost;
	const Tetro& _actual;
};
#endif /* GHOST_TETRO_H */
