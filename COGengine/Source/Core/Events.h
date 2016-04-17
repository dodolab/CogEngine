#pragma once

#include "Definitions.h"
#include "StrId.h"

namespace Cog {

	class InputAct;

	// ========================================================
	// This section contains all events that make a payload
	// of a message
	
	/**
	* Abstract class for all events
	*/
	class MsgEvent {

	};

	/**
	* Type of flag change
	*/
	enum class FlagChangeType {
		RESET, SET, SWITCH
	};

	/**
	* Event invoked when a flag of a node is changed 
	*/
	class FlagChangeEvent : public MsgEvent {
	public:
		FlagChangeType changeType = FlagChangeType::SET;
		unsigned state1 = 0;
		unsigned state2 = 0;

		FlagChangeEvent(FlagChangeType changeType, unsigned state1) 
			: changeType(changeType), state1(state1) {

		}

		FlagChangeEvent(FlagChangeType changeType, unsigned state1, unsigned state2) 
			: changeType(changeType), state1(state1), state2(state2) {

		}
	};

	/**
	* User input event (keyboard click, mouse click or touch)
	*/
	class InputEvent : public MsgEvent {
	public:
		InputAct* input;

		InputEvent(InputAct* input) : input(input) {

		}
	};

	/**
	* Type of attribute change
	*/
	enum class AttrChangeType {
		ADD, MODIFY, REMOVE
	};
	
	/**
	* Event invoked when an attribute was changed
	*/
	class AttributeChangeEvent : public MsgEvent {
	public:
		AttrChangeType changeType;
		StrId attribute;

		AttributeChangeEvent(StrId attribute, AttrChangeType changeType) 
			: attribute(attribute), changeType(changeType) {

		}
	};

	/**
	* Event invoked when a value is changed
	*/
	template <class  T>
	class ValueChangeEvent : public MsgEvent {
	public:
		T before;
		T after;

		ValueChangeEvent(T before, T after):before(before), after(after) {

		}
	};

	class NetInputMessage;

	/**
	* Event invoked when a network message arrived
	*/
	class NetworkMsgEvent : public MsgEvent {
	public:
		spt<NetInputMessage> msg;

		NetworkMsgEvent(spt<NetInputMessage> msg) : msg(msg) {

		}

		~NetworkMsgEvent(){

		}
	};

	/**
	* A common generic event with a value
	*/
	template <class  T>
	class CommonEvent : public MsgEvent {
	public:
		spt<T> value;

		CommonEvent(spt<T> value) :value(value) {

		}
	};



}// namespace