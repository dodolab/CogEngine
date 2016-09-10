#pragma once

#include "Definitions.h"
#include "StrId.h"

namespace Cog {

	class InputAct;

	enum class StateChange{
		RESET, SET, SWITCH
	};
	
	/**
	* Abstract class for all events
	*/
	class MsgEvent {

	};

	class StateChangeEvent : public MsgEvent {
	public:
		StateChange changeType = StateChange::SET;
		unsigned state1 = 0;
		unsigned state2 = 0;

		StateChangeEvent(StateChange changeType, unsigned state1) : changeType(changeType), state1(state1) {

		}

		StateChangeEvent(StateChange changeType, unsigned state1, unsigned state2) : changeType(changeType), state1(state1), state2(state2) {

		}
	};

	class InputEvent : public MsgEvent {
	public:
		InputAct* input;

		InputEvent(InputAct* input) : input(input) {

		}
	};


	enum class AttrChange {
		ADD, MODIFY, REMOVE
	};
	

	class AttributeChangeEvent : public MsgEvent {
	public:
		AttrChange changeType;
		StrId attribute;

		AttributeChangeEvent(StrId attribute, AttrChange changeType) : attribute(attribute), changeType(changeType) {

		}
	};

	template <class  T>
	class ValueChangeEvent : public MsgEvent {
	public:
		T before;
		T after;

		ValueChangeEvent(T before, T after):before(before), after(after) {

		}
	};

	class NetInputMessage;

	class NetworkMsgEvent : public MsgEvent {
	public:
		spt<NetInputMessage> msg;

		NetworkMsgEvent(spt<NetInputMessage> msg) : msg(msg) {

		}

		~NetworkMsgEvent(){

		}
	};

	template <class  T>
	class CommonEvent : public MsgEvent {
	public:
		spt<T> value;

		CommonEvent(spt<T> value) :value(value) {

		}
	};



}// namespace