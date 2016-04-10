#pragma once

#include "Behavior.h"

namespace Cog {

	class StateMachine;

	/**
	* Common behavior for all behavioral states of the finite state machine
	*/
	class State : public Behavior {
	private:
		StrId state = 0;
		StateMachine* parent = nullptr;

	public:

		State() {

		}

		State(StrId state) : state(state){

		}

		StrId GetState() {
			return state;
		}

		void SetState(StrId state) {
			this->state = state;
		}

		StateMachine* GetParent() {
			return parent;
		}

		void SetParent(StateMachine* parent) {
			this->parent = parent;
		}

	};

}// namespace