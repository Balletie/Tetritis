#ifndef LOGIC_H
#define LOGIC_H

#include <queue>
#include <memory>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <thread>
#include "board.h"

class AbstractLogic;

enum class LogicEvent {
	Move,
	Rotation,
	TetroAdded,
	WallHit,
	Drop
};

// Hash specialization for LogicEvent enum.
namespace std {
	template<> struct hash<LogicEvent> {
		size_t operator()(const LogicEvent e) const {
			return static_cast<typename std::underlying_type<LogicEvent>::type>(e);
		}
	};
} // namespace std

class LogicCommand {
  public:
	LogicCommand(AbstractLogic& l) : _logic(l) {}
	virtual void perform() = 0;

	virtual ~LogicCommand() {}
  protected:
	AbstractLogic& _logic;
};

class LogicCommandFactory {
  public:
	LogicCommandFactory(AbstractLogic& l) : _logic(l) {}

	virtual std::shared_ptr<LogicCommand> createMoveCommand(const direction) const = 0;
	virtual std::shared_ptr<LogicCommand> createRotateCommand(const rotation) const = 0;
	virtual std::shared_ptr<LogicCommand> createDropCommand() const = 0;

	virtual ~LogicCommandFactory() {}

  protected:
	AbstractLogic& _logic;
};

class AbstractLogic {
  private:
	/* Wrapper with no template arguments, so that any function type can be
	 * stored in the same container. */
	struct AbstractLogicEventCallback {
		virtual ~AbstractLogicEventCallback() {}
	};

	/* Concrete wrapper which has template arguments. */
	template <typename Func>
	struct LogicEventCallback : AbstractLogicEventCallback {
		public:
		std::function<Func> _function;
		LogicEventCallback(std::function<Func> f) : _function(f) {}
	};

	/* The callbacks are stored grouped by their function signature. */
	typedef std::unordered_multimap<std::type_index, std::unique_ptr<AbstractLogicEventCallback>>
		LogicEventCallbacks;

  public:
	virtual Tetro& getCurrentTetro() = 0;
	virtual Tetro getCurrentTetro() const = 0;
	virtual const Board& getBoard() const = 0;

	void enqueue(std::shared_ptr<LogicCommand>);
	void update();

	virtual void record() = 0;

	/* Adapted from http://stackoverflow.com/a/16884259 */
	template <typename Func>
	void addCallback(LogicEvent id, std::function<Func> func) {
		const std::type_index listeners_index(typeid(Func));
		std::unique_ptr<AbstractLogicEventCallback> func_ptr(new LogicEventCallback<Func>(func));
		LogicEventCallbacks& callbacks = _eventCallbacks[id];
		callbacks.insert(LogicEventCallbacks::value_type(listeners_index, std::move(func_ptr)));
	}

	template <typename... Args>
	void callBack(LogicEvent id, Args&&... args) const {
		typedef void Func(typename std::remove_reference<Args>::type...);

		std::type_index listeners_index(typeid(Func));
		auto cbs = _eventCallbacks.find(id);
		if (cbs == _eventCallbacks.end()) {
			return; // No callbacks found for this type of event.
		}

		auto range = cbs->second.equal_range(listeners_index);
		for (auto it = range.first; it != range.second; it++) {
			AbstractLogicEventCallback& callback = *it->second;
			// This cast is safe because we only store one type of AbstractLogicEventCallback.
			const std::function<Func>& func =
				static_cast<const LogicEventCallback<Func> &>(callback)._function;
			func(std::forward<Args>(args)...);
		}
	}

  protected:
	std::queue<std::shared_ptr<LogicCommand>> _command_queue;

  private:
	std::unordered_map<LogicEvent, LogicEventCallbacks> _eventCallbacks;
};

class Logic : public AbstractLogic {
  public:
	Logic();

	Tetro& getCurrentTetro() override {
		return _current_tetro;
	}

	Tetro getCurrentTetro() const {
		return _current_tetro;
	}

	const Board& getBoard() const {
		return _board;
	}

	void record() override;

	void gravity();
	void toggleGravity() {
		_has_gravity = !_has_gravity;
	}

	~Logic();

	Board _board;
	Tetro _current_tetro;
	std::unique_ptr<LogicCommandFactory> _command_factory;

  private:
	std::thread _gravity_task;
	bool _has_gravity;
};

class BasicMoveCommand : public LogicCommand {
  public:
	BasicMoveCommand(AbstractLogic& l, direction dir)
		: LogicCommand(l), _dir(dir) {}
	void perform() override;

  private:
	direction _dir;
};

class BasicRotateCommand : public LogicCommand {
  public:
	BasicRotateCommand(AbstractLogic& l, rotation rot)
		: LogicCommand(l), _rot(rot) {}

	void perform() override;

  private:
	rotation _rot;
};

class BasicDropCommand : public LogicCommand {
  public:
	BasicDropCommand(AbstractLogic& l) : LogicCommand(l) {}
	void perform() override;
};

class BasicLogicCommandFactory : public LogicCommandFactory {
  public:
	BasicLogicCommandFactory(AbstractLogic& l) : LogicCommandFactory(l) {}
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
