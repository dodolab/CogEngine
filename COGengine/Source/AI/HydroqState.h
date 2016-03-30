#pragma once

#include "AIState.h"

class HydroqState : public AIState {
public:
	int aiRigs;
	int playerRigs;
	int emptyRigs;
	bool aiOnTurn;

	HydroqState(int aiRigs, int playerRigs, int emptyRigs, bool aiOnTurn) : aiRigs(aiRigs), playerRigs(playerRigs), emptyRigs(emptyRigs), aiOnTurn(aiOnTurn)
	{

	}

	virtual int GetAgentOnTurn() {
		return aiOnTurn ? 0 : 1;
	}

	virtual spt<AIState> DeepCopy() {
		return spt<HydroqState>(new HydroqState(aiRigs, playerRigs, emptyRigs, aiOnTurn));
	}

protected:

	virtual bool Equals(AIState* st) {
		auto casted = static_cast<HydroqState*>(st);
		return casted->aiOnTurn == this->aiOnTurn && 
			casted->aiRigs == this->aiRigs && 
			casted->emptyRigs == this->emptyRigs && 
			casted->playerRigs == this->playerRigs;
	}

};