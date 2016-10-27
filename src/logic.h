#ifndef LOGIC_H
#define LOGIC_H

#include <queue>
#include <memory>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <thread>
#include "board.h"

class Logic;

enum class LogicEvent {
	Move,
	Rotation,
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
	LogicCommand(Logic& l) : _logic(l) {}
	virtual void perform() = 0;

	virtual ~LogicCommand() {}
  protected:
	Logic& _logic;
};

class LogicCommandFactory {
  public:
	LogicCommandFactory(Logic& l) : _logic(l) {}

	virtual std::shared_ptr<LogicCommand> createMoveCommand(const direction) const = 0;
	virtual std::shared_ptr<LogicCommand> createRotateCommand(const rotation) const = 0;
	virtual std::shared_ptr<LogicCommand> createDropCommand() const = 0;

	virtual ~LogicCommandFactory() {}

  protected:
	Logic& _logic;
};

class Logic {
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
	Logic();

	void enqueue(std::shared_ptr<LogicCommand> lc) {
		_command_queue.push(std::move(lc));
	}

	void update();
	void record();

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
		if (cbs == _eventCallbacks.end())
			return; // No callbacks found for this type of event.

		auto range = cbs->second.equal_range(listeners_index);
		for (auto it = range.first; it != range.second; it++) {
			AbstractLogicEventCallback& callback = *it->second;
			// This cast is safe because we only store one type of AbstractLogicEventCallback.
			const std::function<Func>& func =
				static_cast<const LogicEventCallback<Func> &>(callback)._function;
			func(std::forward<Args>(args)...);
		}
	}

	void gravity();
	void toggleGravity() {
		_has_gravity = !_has_gravity;
	}

	~Logic();

	Board _board;
	Tetro _current_tetro;
	std::unique_ptr<LogicCommandFactory> _command_factory;

  protected:
	std::queue<std::shared_ptr<LogicCommand>> _command_queue;

  private:
	std::unordered_map<LogicEvent, LogicEventCallbacks> _eventCallbacks;
	std::thread _gravity_task;
	bool _has_gravity;
};

class BasicMoveCommand : public LogicCommand {
  public:
	BasicMoveCommand(Logic& l, direction dir)
		: LogicCommand(l), _dir(dir) {}
	void perform() override;

  private:
	direction _dir;
};

class BasicRotateCommand : public LogicCommand {
  public:
	BasicRotateCommand(Logic& l, rotation rot)
		: LogicCommand(l), _rot(rot) {}

	void perform() override;

  private:
	rotation _rot;
};

class BasicDropCommand : public LogicCommand {
  public:
	BasicDropCommand(Logic& l) : LogicCommand(l) {}
	void perform() override;
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
