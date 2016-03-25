#pragma once

#include "Behavior.h"
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
		vector<State*> globalStates;
		map<StringHash, State*> localStates;

	public:

		void OnInit();

		void OnStart();

		void OnMessage(Msg& msg);

		void AddGlobalState(State* globalState);
		
		void AddLocalState(State* localState);

		/**
		* Changes actual state
		*/
		void ChangeState(State* state);

		/**
		* Changes state according to its key
		*/
		bool ChangeState(StringHash state);

		State* FindLocalState(StringHash state);

		State* GetCurrentState() {
			return currentState;
		}

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:
		void StartState(State* state);
	};

}// namespace