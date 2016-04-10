#pragma once

#include "Behavior.h"
#include "State.h"

namespace Cog {

	/**x
	* Behavior for finite state machine
	*/
	class StateMachine : public Behavior {
	private:
		
		State* currentState = nullptr;
		State* previousState = nullptr;
		vector<State*> globalStates;
		map<StrId, State*> localStates;

	public:

		StateMachine() {

		}

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
		bool ChangeState(StrId state);

		State* FindLocalState(StrId state);

		State* GetCurrentState() {
			return currentState;
		}

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:
		void StartState(State* state);
	};

}// namespace