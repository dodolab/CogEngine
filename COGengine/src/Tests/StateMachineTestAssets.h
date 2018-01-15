#pragma once


#include "ofxCogMain.h"
#include "StateMachine.h"
#include "State.h"
#include "StrId.h"



class MiningState : public State {
public:

	MiningState(StrId state) : State(state) {

	}


	virtual void Update(const uint64 delta, const uint64 absolute) {
		int progress = owner->GetAttr<int>(StrId("PROGRESS"));
		progress += 10;
		cout << "mining ..." << endl;
		if (progress >= 100) {
			cout << "mining finished" << endl;
			this->GetParent()->ChangeState(StrId("REST"));
		}

		owner->ChangeAttr(StrId("PROGRESS"), progress);
	}
};

class RestState : public State {
public:

	RestState(StrId state) : State(state) {

	}

	virtual void Update(const uint64 delta, const uint64 absolute) {

	}
};

class StateSwitcher : public State {
public:
	StrId nextState;

	StateSwitcher(StrId state, StrId nextState) : State(state), nextState(nextState) {

	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		this->GetParent()->ChangeState(nextState);
	}
};