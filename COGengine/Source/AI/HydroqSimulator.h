#pragma once

#include "Simulator.h"
#include "HydroqState.h"
#include "HydroqAction.h"
#include "Error.h"
#include "StringHash.h"

using namespace Cog;


#define AGENT_AI 0
#define AGENT_PLAYER 1


class HydroqSimulator : public Simulator<HydroqState>
{
public:

	HydroqSimulator() {
		this->agentsNumber = 2;
	}

	void InitState() {
		this->actualState = HydroqState(0,0,10, AGENT_AI);
		RecalcPossibleActions();
		this->rewards = AgentsReward(0,0);
	}

	spt<Simulator> DeepCopy() {
		auto copy = spt<HydroqSimulator>(new HydroqSimulator());
		copy->actualState = this->actualState;
		copy->agentsNumber = this->agentsNumber;
		copy->possibleActions = this->possibleActions;
		copy->rewards = this->rewards;
		return copy;
	}

	static int mojoCounter;

	void MakeAction(StringHash action) {
		if (find(this->possibleActions.begin(), this->possibleActions.end(), action) == this->possibleActions.end()) {
			throw IllegalOperationException("Wrong action to take!");
		}

		//cout << (mojoCounter++) << endl;

		if (action == (unsigned)HydroqAction::TAKE_EMPTY_RIG) {
			if (this->actualState.GetAgentOnTurn() == AGENT_AI) {
				this->actualState.aiRigs++;
				this->actualState.emptyRigs--;
				rewards = AgentsReward(5, 0);
			//	cout << " AI TAKE EMPTY RIG : " << this->actualState.aiRigs << ":" << actualState.playerRigs << ":" << actualState.emptyRigs << endl;
			}
			else {
				this->actualState.playerRigs++;
				this->actualState.emptyRigs--;
				rewards = AgentsReward(0, 5);
			//	cout << " PLAYER TAKE EMPTY RIG : " << this->actualState.aiRigs << ":" << actualState.playerRigs << ":" << actualState.emptyRigs << endl;
			}
		}
		else if (action == (unsigned)HydroqAction::TAKE_ENEMY_RIG) {
			if (this->actualState.GetAgentOnTurn() == AGENT_AI) {
				this->actualState.aiRigs++;
				this->actualState.playerRigs--;
				rewards = AgentsReward(10, 0);
			//	cout << " AI TAKE ENEMY RIG : " << this->actualState.aiRigs << ":" << actualState.playerRigs << ":" << actualState.emptyRigs << endl;
			}
			else {
				this->actualState.aiRigs--;
				this->actualState.playerRigs++;
				rewards = AgentsReward(0, 10);
			//	cout << " PLAYER TAKE ENEMY RIG : " << this->actualState.aiRigs << ":" << actualState.playerRigs << ":" << actualState.emptyRigs << endl;
			}
		}
		else if (action == (unsigned)HydroqAction::WAIT) {
			if (this->actualState.GetAgentOnTurn() == AGENT_AI) {
				rewards = AgentsReward(0, 0);
			//	cout << " AI WAITS : " << this->actualState.aiRigs << ":" << actualState.playerRigs << ":" << actualState.emptyRigs << endl;
			}
			else {
				rewards = AgentsReward(0, 0);
			//	cout << " PLAYER WAITS : " << this->actualState.aiRigs << ":" << actualState.playerRigs << ":" << actualState.emptyRigs << endl;
			}
		}

		this->actualState.SwapAgentOnTurn();
		RecalcPossibleActions();
	}

protected:
	virtual void RecalcPossibleActions() {
		this->possibleActions.clear();

		if (actualState.GetAgentOnTurn() == AGENT_AI) {
			if (actualState.aiRigs == 0 && actualState.emptyRigs == 0) return;
			if (actualState.playerRigs > 0) possibleActions.push_back(StringHash((unsigned)HydroqAction::TAKE_ENEMY_RIG));
			possibleActions.push_back(StringHash((unsigned)HydroqAction::WAIT));
			if (actualState.emptyRigs > 0) possibleActions.push_back(StringHash((unsigned)HydroqAction::TAKE_EMPTY_RIG));
		}
		else {
			if (actualState.playerRigs == 0 && actualState.emptyRigs == 0) return;
			if (actualState.aiRigs > 0) possibleActions.push_back(StringHash((unsigned)HydroqAction::TAKE_ENEMY_RIG));
			possibleActions.push_back(StringHash((unsigned)HydroqAction::WAIT));
			if (actualState.emptyRigs > 0) possibleActions.push_back(StringHash((unsigned)HydroqAction::TAKE_EMPTY_RIG));
			
		}
	}

};

int HydroqSimulator::mojoCounter = 0;