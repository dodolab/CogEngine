#pragma once

#include "AIAgent.h"
#include "ofMath.h"

template<class S, class A>
class RandomAgent : public AIAgent<S,A> {
protected:
public:
	
	RandomAgent() {

	}

	RandomAgent(string name) : AIAgent(name) {
		
	}

	virtual A ChooseAction(spt<Simulator<S, A>> simulator) {
		auto actions = simulator->GetPossibleActions();
		return actions[(int)ofRandom(0, actions.size())];

	}

};