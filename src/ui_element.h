#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <SFML/Graphics.hpp>

class UIElement : sf::Drawable {
  public:
	UIElement(sf::RenderTarget& target)
		: _target(target)
	{}

	virtual const sf::View& getView() const {
		return _target.getDefaultView();
	}

	virtual void update() {
		_target.setView(getView());
	}

  protected:
	sf::RenderTarget& _target;
};

#endif /* UI_ELEMENT_H */
