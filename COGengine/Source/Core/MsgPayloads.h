#pragma once

#include "Definitions.h"
#include "StrId.h"

namespace Cog {

	class InputAct;

	// ========================================================
	// This section contains all objects that make a mesage payload
	
	/**
	* Abstract class for all payloads
	*/
	class MsgPayload {

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
	class FlagChangeEvent : public MsgPayload {
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
	class InputEvent : public MsgPayload {
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
	class AttributeChangeEvent : public MsgPayload {
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
	class ValueChangeEvent : public MsgPayload {
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
	class NetworkMsgEvent : public MsgPayload {
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
	class CommonEvent : public MsgPayload {
	public:
		T value;

		CommonEvent(T value) :value(value) {

		}
	};



}// namespace