template <class SomeLogic>
void BasicMoveCommand<SomeLogic>::perform() {
	Tetro t = logic_access::getCurrentTetro(this->_logic);
	t.move(_dir);
	if (!logic_access::getBoard(this->_logic).isOutOfSideBounds(t)) {
		if (logic_access::getBoard(this->_logic).collides(t)) {
			if (_dir == DIR_DOWN) {
				logic_access::record(this->_logic);
			}
		} else {
			logic_access::getCurrentTetro(this->_logic) = t;
			this->_logic.callBack(LogicEvent::Move, _dir);
		}
	} else {
		this->_logic.callBack(LogicEvent::WallHit, t);
	}
}

template <class SomeLogic>
void BasicRotateCommand<SomeLogic>::perform() {
	for (int i = 0; i < 5; i++) {
		Tetro t = logic_access::getCurrentTetro(this->_logic);
		Tetro::WallKickTranslation trans = t.rotate(_rot, i);

		if (!logic_access::getBoard(this->_logic).isOutOfSideBounds(t) && !logic_access::getBoard(this->_logic).collides(t)) {
			Tetro::WallKickOffset off = t.getWallKickOffset(i);
			logic_access::getCurrentTetro(this->_logic) = t;
			this->_logic.callBack(LogicEvent::Rotation, _rot, trans, off);
			return;
		}
	}
	this->_logic.callBack(LogicEvent::WallHit, logic_access::getCurrentTetro(this->_logic));
}

template <class SomeLogic>
void BasicDropCommand<SomeLogic>::perform() {
	Tetro& t = logic_access::getCurrentTetro(this->_logic);
	Tetro cur = t;
	uint8_t i = 0;
	while (!logic_access::getBoard(this->_logic).collides(cur)) {
		t = cur;
		cur.move(DIR_DOWN);
		i++;
	}
	logic_access::record(this->_logic);
	this->_logic.callBack(LogicEvent::Drop, (uint8_t) (i - 1));
}