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

template <class SomeLogic>
class LogicCommand {
  public:
	LogicCommand(SomeLogic& l) : _logic(l) {}
	virtual void perform() = 0;

	virtual ~LogicCommand() {}
  protected:
	SomeLogic& _logic;
};

template <class SomeLogic>
class LogicCommandFactory {
  public:
	LogicCommandFactory(SomeLogic& l) : _logic(l) {}

	virtual std::shared_ptr<LogicCommand<SomeLogic>> createMoveCommand(const direction) const = 0;
	virtual std::shared_ptr<LogicCommand<SomeLogic>> createRotateCommand(const rotation) const = 0;
	virtual std::shared_ptr<LogicCommand<SomeLogic>> createDropCommand() const = 0;

	virtual ~LogicCommandFactory() {}

  protected:
	SomeLogic& _logic;
};

namespace detail {

template <class DerivedLogic>
class LogicBase {
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
	void enqueue(std::shared_ptr<LogicCommand<DerivedLogic>> lc) {
		_command_queue.push(std::move(lc));
	}

	void update() {
		while (!_command_queue.empty()) {
			std::shared_ptr<LogicCommand<DerivedLogic>> c = _command_queue.front();
			c->perform();
			_command_queue.pop();
		}
	}

	/* Adapted from http://stackoverflow.com/a/16884259 */
	template <typename Func>
	void addCallback(LogicEvent id, std::function<Func> func) {
		const std::type_index listeners_index(typeid(Func));
		std::unique_ptr<AbstractLogicEventCallback> func_ptr(new LogicEventCallback<Func>(func));
		LogicEventCallbacks& callbacks = _eventCallbacks[id];
		callbacks.insert(typename LogicEventCallbacks::value_type(listeners_index, std::move(func_ptr)));
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
	std::queue<std::shared_ptr<LogicCommand<DerivedLogic>>> _command_queue;

  private:
	std::unordered_map<LogicEvent, LogicEventCallbacks> _eventCallbacks;
};
} // namespace detail

class logic_access {
  public:
	// No instances have to be made for this class.
	logic_access() = delete;

	template <class SomeLogic>
	static Tetro& getCurrentTetro(SomeLogic& l) {
		return l.getCurrentTetro_int();
	}

	template <class SomeLogic>
	static Board& getBoard(SomeLogic& l) {
		return l.getBoard_int();
	}

	template <class SomeLogic>
	static void record(SomeLogic& l) {
		l.record();
	}
};

class Logic : public detail::LogicBase<Logic> {
  public:
	using tetro_factory = GuidelineTetroFactory;

	friend logic_access;

	Logic();

	const tetro_factory& getFactory() const {
		return *_tetro_factory;
	}

	const Board& getBoard() const {
		return _board;
	}

	const Tetro& getCurrentTetro() const {
		return _tetro;
	}

	const Tetro& getCurrentTetro() {
		return getCurrentTetro_int();
	}

	void record();

	void gravity();
	void toggleGravity() {
		_has_gravity = !_has_gravity;
	}

	~Logic();

	std::unique_ptr<LogicCommandFactory<Logic>> _command_factory;

  private:
	Tetro& getCurrentTetro_int() {
		return _tetro;
	}

	Board& getBoard_int() {
		return _board;
	}

	std::thread _gravity_task;
	bool _has_gravity;
	std::unique_ptr<GuidelineTetroFactory> _tetro_factory;

	Tetro _tetro;
	Board _board;
};

template <class SomeLogic>
class BasicMoveCommand : public LogicCommand<SomeLogic> {
  public:
	BasicMoveCommand(SomeLogic& l, direction dir)
		: LogicCommand<SomeLogic>(l), _dir(dir) {}
	void perform() override;

  private:
	direction _dir;
};

template <class SomeLogic>
class BasicRotateCommand : public LogicCommand<SomeLogic> {
  public:
	BasicRotateCommand(SomeLogic& l, rotation rot)
		: LogicCommand<SomeLogic>(l), _rot(rot) {}

	void perform() override;

  private:
	rotation _rot;
};

template <class SomeLogic>
class BasicDropCommand : public LogicCommand<SomeLogic> {
  public:
	BasicDropCommand(SomeLogic& l) : LogicCommand<SomeLogic>(l) {}
	void perform() override;
};

template <class SomeLogic>
class BasicLogicCommandFactory : public LogicCommandFactory<SomeLogic> {
	using SomeLogicCommand = LogicCommand<SomeLogic>;
	using SomeLogicCommandFactory = LogicCommandFactory<SomeLogic>;

  public:
	BasicLogicCommandFactory(SomeLogic& l) : SomeLogicCommandFactory(l) {}
	virtual std::shared_ptr<SomeLogicCommand> createMoveCommand(const direction dir) const {
		return std::shared_ptr<SomeLogicCommand>(new BasicMoveCommand<SomeLogic>(this->_logic, dir));
	}

	virtual std::shared_ptr<SomeLogicCommand> createRotateCommand(const rotation rot) const {
		return std::shared_ptr<SomeLogicCommand>(new BasicRotateCommand<SomeLogic>(this->_logic, rot));
	}

	virtual std::shared_ptr<SomeLogicCommand> createDropCommand() const {
		return std::shared_ptr<SomeLogicCommand>(new BasicDropCommand<SomeLogic>(this->_logic));
	}
};

#include "basic_commands.tpp"

#endif /* LOGIC_H */
