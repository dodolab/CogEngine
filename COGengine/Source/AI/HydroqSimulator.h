#pragma once

#include "Simulator.h"
#include "HydroqState.h"
#include "HydroqAction.h"
#include "Error.h"

class HydroqSimulator : public Simulator<HydroqState, HydroqAction>
{
public:
	HydroqSimulator() {
		this->agentsNumber = 2;
	}

	void InitState() {
		this->actualState = spt<HydroqState>(new HydroqState(0,0,10, true));
		RecalcPossibleActions();
		this->rewards = AgentsReward(0,0);
	}

	spt<Simulator> DeepCopy() {
		auto copy = spt<HydroqSimulator>(new HydroqSimulator());
		copy->actualState =  this->actualState->DeepCopy();
		copy->agentsNumber = this->agentsNumber;
		copy->possibleActions = this->possibleActions;
		copy->rewards = this->rewards;
		return copy;
	}

	void MakeAction(HydroqAction action) {
		if (find(this->possibleActions.begin(), this->possibleActions.end(), action) == this->possibleActions.end()) {
			throw IllegalOperationException("Wrong action to take!");
		}

		
		this->actualState = this->actualState->DeepCopy();
		this->actualState->aiOnTurn = !this->actualState->aiOnTurn;

		if (action == HydroqAction::TAKE_EMPTY_RIG) {
			if (this->actualState->aiOnTurn) {
				this->actualState->aiRigs++;
				this->actualState->emptyRigs--;
				rewards = AgentsReward(5, 0);
				cout << " AI TAKE EMPTY RIG : " << this->actualState->aiRigs << ":" << actualState->playerRigs << ":" << actualState->emptyRigs << endl;
			}
			else {
				this->actualState->playerRigs++;
				this->actualState->emptyRigs--;
				rewards = AgentsReward(0, 5);
				cout << " PLAYER TAKE EMPTY RIG : " << this->actualState->aiRigs << ":" << actualState->playerRigs << ":" << actualState->emptyRigs << endl;
			}
		}
		else if (action == HydroqAction::TAKE_ENEMY_RIG) {
			if (this->actualState->aiOnTurn) {
				this->actualState->aiRigs++;
				this->actualState->playerRigs--;
				rewards = AgentsReward(10, 0);
				cout << " AI TAKE ENEMY RIG : " << this->actualState->aiRigs << ":" << actualState->playerRigs << ":" << actualState->emptyRigs << endl;
			}
			else {
				this->actualState->aiRigs--;
				this->actualState->playerRigs++;
				rewards = AgentsReward(0, 10);
				cout << " PLAYER TAKE ENEMY RIG : " << this->actualState->aiRigs << ":" << actualState->playerRigs << ":" << actualState->emptyRigs << endl;
			}
		}
		else if (action == HydroqAction::WAIT) {
			if (this->actualState->aiOnTurn) {
				rewards = AgentsReward(-1, 0);
				cout << " AI WAITS : " << this->actualState->aiRigs << ":" << actualState->playerRigs << ":" << actualState->emptyRigs << endl;
			}
			else {
				rewards = AgentsReward(0, -1);
				cout << " PLAYER WAITS : " << this->actualState->aiRigs << ":" << actualState->playerRigs << ":" << actualState->emptyRigs << endl;
			}
		}

		RecalcPossibleActions();
	}

protected:
	virtual void RecalcPossibleActions() {
		this->possibleActions.clear();

		if (actualState->aiOnTurn) {
			if (actualState->aiRigs == 0 && actualState->emptyRigs == 0) return;
			if (actualState->playerRigs > 0) possibleActions.push_back(HydroqAction::TAKE_ENEMY_RIG);
			possibleActions.push_back(HydroqAction::WAIT);
			if (actualState->emptyRigs > 0) possibleActions.push_back(HydroqAction::TAKE_EMPTY_RIG);
		}
		else {
			if (actualState->playerRigs == 0 && actualState->emptyRigs == 0) return;
			if (actualState->aiRigs > 0) possibleActions.push_back(HydroqAction::TAKE_ENEMY_RIG);
			possibleActions.push_back(HydroqAction::WAIT);
			if (actualState->emptyRigs > 0) possibleActions.push_back(HydroqAction::TAKE_EMPTY_RIG);
			
		}
	}

};