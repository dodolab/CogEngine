
#include "StateMachine.h"
#include "CogEngine.h"

namespace Cog {


	void StateMachine::OnInit() {
		for (auto& globalState : globalStates) {
			SetOwner(globalState, owner);
			globalState->Init();
		}

		for (auto& localState : localStates) {
			SetOwner(localState.second, owner);
			localState.second->Init();
		}
	}

	void StateMachine::OnStart() {

		for (auto& globalState : globalStates) {
			globalState->Start();
		}

		if (currentState != nullptr) {
			StartState(currentState);
		}
	}

	void StateMachine::OnMessage(Msg& msg) {
		if (msg.GetSourceObject()->GetId() == owner->GetId()) {

			for (auto& globalState : globalStates) {
				globalState->OnMessage(msg);
			}

			if (currentState != nullptr) currentState->OnMessage(msg);
		}
	}

	void StateMachine::AddGlobalState(State* globalState) {
		for (auto& global : globalStates) {
			if (global->GetState() == globalState->GetState()) {
				CogLogDebug("StateMachine", "An attempt to add already added global state");
				return;
			}
		}

		globalStates.push_back(globalState);
		globalState->SetParent(this);

		if (this->initialized && !globalState->IsInitialized()) {
			globalState->Init();
		}
	}

	void StateMachine::AddLocalState(State* localState) {
		for (auto& local : localStates) {
			if (local.second->GetState() == localState->GetState()) {
				CogLogDebug("StateMachine", "An attempt to add already added local state");
				return;
			}
		}

		localStates[localState->GetState()] = localState;
		localState->SetParent(this);

		if (this->initialized && !localState->IsInitialized()) {
			localState->Init();
		}
	}

	/**
	* Changes actual state
	*/
	void StateMachine::ChangeState(State* state) {
		this->previousState = currentState;

		if (this->currentState != nullptr) this->currentState->Finish();

		this->currentState = state;

		// add the state into map
		if (localStates.count(state->GetState()) == 0) {
			localStates[state->GetState()] = state;
		}

		state->SetParent(this);

		if (this->initialized && !state->IsInitialized()) {
			state->Init();
		}

		if (this->initialized) {
			state->Start();
		}
	}

	/**
	* Changes state according to its key
	*/
	bool StateMachine::ChangeState(StrId state) {
		if (localStates.count(state) != 0) {
			this->ChangeState(localStates.find(state)->second);
			return true;
		}
		return false;
	}

	State* StateMachine::FindLocalState(StrId state) {
		auto found = localStates.find(state);
		if (found != localStates.end()) return found->second;
		else return nullptr;
	}

	void StateMachine::Update(const uint64 delta, const uint64 absolute) {
		for (auto& global : globalStates) {
			global->Update(delta, absolute);
		}

		if (currentState != nullptr) currentState->Update(delta, absolute);
	}

	void StateMachine::StartState(State* state) {
		SetOwner(state, owner);
		state->Start();
	}

}// namespace