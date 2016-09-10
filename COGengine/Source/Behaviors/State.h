#pragma once

#include "ofxCogCommon.h"
#include "Behavior.h"

namespace Cog {

	class StateMachine;

	/**
	* Common behavior for all behavioral states of the finite state machine
	*/
	class State : public Behavior {
	private:
		StringHash state = 0;
		StateMachine* parent = nullptr;

	public:

		State(StringHash state) : state(state){

		}

		StringHash GetState() {
			return state;
		}

		void SetState(StringHash state) {
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