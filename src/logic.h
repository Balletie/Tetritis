#ifndef LOGIC_H
#define LOGIC_H

#include <queue>
#include <memory>
#include "board.h"

class Logic;

class LogicCommand {
  public:
	LogicCommand(Logic& l) : _logic(l) {};
	virtual void perform() = 0;

  protected:
	Logic& _logic;
};

class LogicCommandFactory {
  public:
	LogicCommandFactory(Logic& l) : _logic(l) {}
	virtual std::shared_ptr<LogicCommand> createMoveCommand(const direction) const = 0;
	virtual std::shared_ptr<LogicCommand> createRotateCommand(const rotation) const = 0;
	virtual std::shared_ptr<LogicCommand> createDropCommand() const = 0;

  protected:
	Logic& _logic;
};

class Logic : public sf::Drawable {
  public:
	Logic();

	void enqueue(std::shared_ptr<LogicCommand> lc) {
		_command_queue.push(std::move(lc));
	}

	void update();

	Board _board;
	Tetro _current_tetro;
	std::unique_ptr<LogicCommandFactory> _command_factory;

	private:
	void emptyCommandQueue();

  protected:
	void draw(sf::RenderTarget&, sf::RenderStates) const;

	std::queue<std::shared_ptr<LogicCommand>> _command_queue;
};

class BasicMoveCommand : public LogicCommand {
  public:
	BasicMoveCommand(Logic& l, direction dir) : LogicCommand(l), _dir(dir) {}
	void perform();

  private:
	direction _dir;
};

class BasicRotateCommand : public LogicCommand {
  public:
	BasicRotateCommand(Logic& l, rotation rot) : LogicCommand(l), _rot(rot) {}
	void perform();

  private:
	rotation _rot;
};

class BasicDropCommand : public LogicCommand {
  public:
	BasicDropCommand(Logic& l) : LogicCommand(l) {}
	void perform();
};

class BasicLogicCommandFactory : public LogicCommandFactory {
  public:
	BasicLogicCommandFactory(Logic& l) : LogicCommandFactory(l) {}
	virtual std::shared_ptr<LogicCommand> createMoveCommand(const direction dir) const {
		return std::shared_ptr<LogicCommand>(new BasicMoveCommand(_logic, dir));
	}

	virtual std::shared_ptr<LogicCommand> createRotateCommand(const rotation rot) const {
		return std::shared_ptr<LogicCommand>(new BasicRotateCommand(_logic, rot));
	}

	virtual std::shared_ptr<LogicCommand> createDropCommand() const {
		return std::shared_ptr<LogicCommand>(new BasicDropCommand(_logic));
	}
};

#endif /* LOGIC_H */
