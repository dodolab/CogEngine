#pragma once

#include <vector>
using namespace std;

#include "AgentsReward.h"
#include "ofxSmartPointer.h"
#include "StringHash.h"

using namespace Cog;

template<class S>
class Simulator {
protected:
	S actualState;
	vector<StringHash> possibleActions;
	AgentsReward rewards;
	int agentsNumber;

public:

	Simulator() {

	}

	virtual void InitState() = 0;

	S& GetActualState() {
		return actualState;
	}

	virtual spt<Simulator> DeepCopy() = 0;

	void SetActualState(S state) {
		this->actualState = state;
		RecalcPossibleActions();
	}

	void SetActualState(S state, vector<StringHash> possibleActions) {
		this->actualState = state;
		this->possibleActions = possibleActions;
		this->rewards = AgentsReward(agentsNumber);
	}

	virtual void MakeAction(StringHash action) = 0;

	vector<StringHash>& GetPossibleActions() {
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