#pragma once

#include "ofxSmartPointer.h"

/**
* State for AI simulation
* Each derived class must have equality operator overidden
*/
class AIState{
protected:
	int agentOnTurn = 0;

public:
	int GetAgentOnTurn() const {
		return agentOnTurn;
	}

	void SetAgentOnTurn(int agentOnTurn) {
		this->agentOnTurn = agentOnTurn;
	}

	void SwapAgentOnTurn(int maxAgents) {
		agentOnTurn = (agentOnTurn + 1) % maxAgents;
	}
};