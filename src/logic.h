#ifndef LOGIC_H
#define LOGIC_H

#include <queue>
#include <memory>
#include "board.h"

class Logic;

typedef std::function<void(direction)> OnMoved;
typedef std::function<void(rotation, Tetro::WallKickTranslation,
	Tetro::WallKickOffset)> OnRotated;
typedef std::function<void(Tetro)> OnWallHit;
typedef std::function<void(uint8_t)> OnDropped;

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

	virtual std::shared_ptr<LogicCommand> createMoveCommand(
		const direction, OnMoved, OnWallHit) const = 0;
	virtual std::shared_ptr<LogicCommand> createRotateCommand(
		const rotation, OnRotated, OnWallHit) const = 0;
	virtual std::shared_ptr<LogicCommand> createDropCommand(OnDropped) const = 0;

  protected:
	Logic& _logic;
};

class Logic {
  public:
	Logic();

	void enqueue(std::shared_ptr<LogicCommand> lc) {
		_command_queue.push(std::move(lc));
	}

	void update();
	void recordOnCollision(Tetro&);
	void record();

	Board _board;
	Tetro _current_tetro;
	std::unique_ptr<LogicCommandFactory> _command_factory;

  protected:
	std::queue<std::shared_ptr<LogicCommand>> _command_queue;
};

class BasicMoveCommand : public LogicCommand {
  public:
	BasicMoveCommand(Logic& l, direction dir, OnMoved r, OnWallHit wh)
		: LogicCommand(l), _dir(dir)
		, _onMovedCallback(r), _onWallHitCallback(wh) {}
	void perform() override;

  private:
	direction _dir;
	OnMoved _onMovedCallback;
	OnWallHit _onWallHitCallback;
};

class BasicRotateCommand : public LogicCommand {
  public:
	BasicRotateCommand(Logic& l, rotation rot, OnRotated r, OnWallHit wh)
		: LogicCommand(l), _rot(rot)
		, _onRotatedCallback(r), _onWallHitCallback(wh) {}

	void perform() override;

  private:
	rotation _rot;
	OnRotated _onRotatedCallback;
	OnWallHit _onWallHitCallback;
};

class BasicDropCommand : public LogicCommand {
  public:
	BasicDropCommand(Logic& l, OnDropped d) : LogicCommand(l), _onDroppedCallback(d) {}
	void perform() override;

  private:
	OnDropped _onDroppedCallback;
};

class BasicLogicCommandFactory : public LogicCommandFactory {
  public:
	BasicLogicCommandFactory(Logic& l) : LogicCommandFactory(l) {}
	virtual std::shared_ptr<LogicCommand> createMoveCommand(const direction dir,
			OnMoved m, OnWallHit wh) const {
		return std::shared_ptr<LogicCommand>(new BasicMoveCommand(_logic, dir, m, wh));
	}

	virtual std::shared_ptr<LogicCommand> createRotateCommand(const rotation rot,
			OnRotated r, OnWallHit wh) const {
		return std::shared_ptr<LogicCommand>(new BasicRotateCommand(_logic, rot, r, wh));
	}

	virtual std::shared_ptr<LogicCommand> createDropCommand(OnDropped d) const {
		return std::shared_ptr<LogicCommand>(new BasicDropCommand(_logic, d));
	}
};

#endif /* LOGIC_H */
