#pragma once

#include <vector>
using namespace std;

#include "AgentsReward.h"
#include "ofxSmartPointer.h"

template<class S, class A>
class Simulator {
protected:
	spt<S> actualState;
	vector<A> possibleActions;
	AgentsReward rewards;
	int agentsNumber;

public:

	Simulator() {

	}

	virtual void InitState() = 0;

	spt<S> GetActualState() {
		return actualState;
	}

	virtual spt<Simulator> DeepCopy() = 0;

	void SetActualState(spt<S> state) {
		this->actualState = state;
		RecalcPossibleActions();
	}

	virtual void MakeAction(A action) = 0;

	vector<A>& GetPossibleActions() {
		return possibleActions;
	}

	AgentsReward& GetRewards() {
		return rewards;
	}


	bool IsDeadEnd() {
		return possibleActions.size() == 0;
	}

	int GetAgentsNumber() {
		return agentsNumber;
	}

protected:
	virtual void RecalcPossibleActions() = 0;
};