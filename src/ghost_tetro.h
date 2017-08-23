#ifndef GHOST_TETRO_H
#define GHOST_TETRO_H

#include "animation.h"
#include "tetro.h"

class GhostLogic : public detail::LogicBase<GhostLogic> {
  public:
	friend logic_access;

	GhostLogic(Tetro& ghost, Board& board)
		: _board(board), _ghost(ghost) {
	}

  private:
	Tetro& getCurrentTetro_int() {
		return _ghost;
	}

	Board& getBoard_int() {
		return _board;
	}

	void record() {
		// Do nothing.
	}

	Board& _board;
	Tetro& _ghost;
};

class GhostTetro : public AnimatedDrawable {
  public:

	REGISTER_CALLBACK(onMoved, direction)
	REGISTER_CALLBACK(onRotated, rotation, Tetro::WallKickTranslation,
		Tetro::WallKickOffset)
	REGISTER_CALLBACK(onDropped, uint8_t)

	template <class SomeLogic>
	static GhostTetro *make_ghost(SomeLogic& l) {
		GhostTetro *ghost = new GhostTetro(logic_access::getCurrentTetro(l),
																			 logic_access::getBoard(l));
		l.addCallback(LogicEvent::Move, ghost->onMoved_cb());
		l.addCallback(LogicEvent::Rotation, ghost->onRotated_cb());
		l.addCallback(LogicEvent::Drop, ghost->onDropped_cb());
		return ghost;
	}

  protected:
	GhostTetro(Tetro&, Board&);

	void step(uint32_t dt) override;

	void updateGhost();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
	std::shared_ptr<LogicCommand<GhostLogic>> _drop_command;
	GhostLogic _gl;
	const Tetro& _actual;
	Tetro _ghost;
};
#endif /* GHOST_TETRO_H */
