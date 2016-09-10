#pragma once


namespace Cog {
	
	enum class StateChange{
		RESET, SET, SWITCH
	};
	
	/**
	* Abstract class for all events
	*/
	class MsgEvent {

	};

	class AttrChangeEvent : public MsgEvent {
	public:
		StateChange changeType = StateChange::SET;
		unsigned state1 = 0;
		unsigned state2 = 0;

		AttrChangeEvent(StateChange changeType, unsigned state1) : changeType(changeType), state1(state1) {

		}

		AttrChangeEvent(StateChange changeType, unsigned state1, unsigned state2) : changeType(changeType), state1(state1), state2(state2) {

		}
	};

}// namespace