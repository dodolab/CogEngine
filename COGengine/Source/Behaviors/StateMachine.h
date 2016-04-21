#pragma once

#include "Behavior.h"
#include "State.h"

namespace Cog {

	/**
	* Behavior for finite state machine,
	* contains collection of global and local states
	* Global states are active the whole time, whereas the object may
	* be in only one local state at a time
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

		/**
		* Finds local state by its key
		*/
		State* FindLocalState(StrId state);

		State* GetCurrentState() const {
			return currentState;
		}

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:
		/**
		* Starts actual state
		*/
		void StartState(State* state);
	};

}// namespace