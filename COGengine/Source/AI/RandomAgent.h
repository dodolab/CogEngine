#pragma once

#include "AIAgent.h"
#include "ofMath.h"

template<class S>
class RandomAgent : public AIAgent<S> {
protected:
public:
	
	RandomAgent() {

	}

	RandomAgent(string name) : AIAgent(name) {
		
	}

	virtual StringHash ChooseAction(spt<Simulator<S>> simulator) {
		auto actions = simulator->GetPossibleActions();
		int randIndex = (int)ofRandom(0, actions.size());
		return actions[randIndex];

	}

};