#ifndef ANIMATION_H
#define ANIMATION_H
#include "macros.h"
#include "logic.h"
#include "tweeny.h"

class AnimatedDrawable : public sf::Drawable {
  public:
	virtual void step(uint32_t) = 0;

	virtual ~AnimatedDrawable() {};
};

template<typename Dr, typename T, typename... Ts>
class TransformingDrawable : public AnimatedDrawable {
	static_assert(std::is_base_of<sf::Drawable, Dr>::value, "sf::Drawable should be a base of Dr");

  public:
	TransformingDrawable(Dr& drawable, tweeny::tween<T, Ts...> t)
		: _drawable(drawable), _tween(t) {};
	void step(uint32_t dt) {
		_tween.step(dt);
	}

  protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform = this->_transform;
		target.draw(_drawable, states);
	}

  protected:
	sf::Transform _transform;
	const Dr& _drawable;
	tweeny::tween<T, Ts...> _tween;
};

class AnimatedTetro : public TransformingDrawable<Tetro, float, float> {
  public:
	AnimatedTetro(Tetro& t) : TransformingDrawable(t, tweeny::from(0.f,0.f).to(0.f,0.f).during(1)) {};

	REGISTER_CALLBACK(onMoved, direction)
	REGISTER_CALLBACK(onRotated, rotation, Tetro::WallKickTranslation,
		Tetro::WallKickOffset)
	REGISTER_CALLBACK(onDropped, uint8_t)

  private:
	void onMoved(direction, uint8_t);
};

class AnimatedBoard : public AnimatedDrawable {
  public:
	AnimatedBoard(const Board& b) : _board(b)
		, _remaining_boards(constructRemainingBoards())
		, _opacity_tween(tweeny::from(255).to(-1).during(200))
	{
		_opacity_tween.seek(1.f);
	};

	typedef std::map<uint8_t, Board> RemovedBoards;

	REGISTER_CALLBACK(onTetroAdded, RemovedBoards)

	void step(uint32_t) override;

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

  private:
	void drawRemoved(sf::RenderTarget&, sf::RenderStates) const;
	void drawRemaining(sf::RenderTarget&, sf::RenderStates) const;
	void addTweenForHeight(uint8_t);
	std::map<uint8_t, Board> constructRemainingBoards();

	const Board& _board;
	std::map<uint8_t, Board> _removed_boards;
	std::map<uint8_t, Board> _remaining_boards;
	std::vector<tweeny::tween<float>> _tweens;
	tweeny::tween<int> _opacity_tween;
};

#endif /* ANIMATION_H */
