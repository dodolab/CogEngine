#pragma once

#include "ofxCogCommon.h"
#include "Behavior.h"

namespace Cog {

	/**
	* Common behavior for all behavioral states of the finite state machine
	*/
	class State : public Behavior {
	private:
		unsigned state = 0;

	public:

		/**
		* Executes the state behavior
		*/
		virtual void Execute(Node* owner) = 0;

		unsigned GetState() {
			return state;
		}

		void SetState(unsigned state) {
			this->state = state;
		}

	};

}// namespace