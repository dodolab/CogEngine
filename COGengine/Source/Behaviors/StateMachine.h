#pragma once

#include "Behavior.h"
#include "Scene.h"
#include "State.h"

namespace Cog {

	/**x
	* Behavior for finite state machine
	*/
	class StateMachine : public Behavior {
		OBJECT_PROTOTYPE(StateMachine)
	private:
		State* currentState = nullptr;
		State* previousState = nullptr;
		State* globalState = nullptr;
	public:


		StateMachine()  {

		}


		virtual void Update(const uint64 delta, const uint64 absolute) {
			if (globalState != nullptr) globalState->Update(delta, absolute);
			if (currentState != nullptr) currentState->Update(delta, absolute);
		}
	};

}// namespace