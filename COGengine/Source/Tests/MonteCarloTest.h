#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;
#include "Vec2i.h"
#include "MonteCarloSearch.h"
#include "RandomAgent.h"
#include "UCTAgent.h"

enum class MCTestAction {
	GO_LEFT = 1,
	GO_RIGHT = 2,
	GO_UP = 3,
	GO_DOWN = 4
};

class MCTestState : public AIState {
public:
	Vec2i player1Pos;
	Vec2i player2Pos;

	MCTestState() {

	}

	MCTestState(int agentOnTurn) {
		this->agentOnTurn = agentOnTurn;
	}

	MCTestState(const MCTestState& copy) {
		this->agentOnTurn = agentOnTurn;
		this->player1Pos = copy.player1Pos;
		this->player2Pos = copy.player2Pos;
	}

	inline bool operator==(const MCTestState& a) const { return player1Pos == a.player1Pos && player2Pos == a.player2Pos; }
	inline bool operator!=(const MCTestState& a) const { return !(*this == a); }
};

class MCTestSimulator : public Simulator<MCTestState, MCTestAction>
{
public:
	
	MCTestSimulator() {
		this->agentsNumber = 2;
	}

	void InitState() {
		this->actualState = MCTestState();
		this->actualState.player1Pos = Vec2i(0, 0);
		this->actualState.player2Pos = Vec2i(10, 10);
		this->rewards = AgentsReward(0,0);
		RecalcPossibleActions();
	}

	spt<Simulator> DeepCopy() {
		auto copy = spt<MCTestSimulator>(new MCTestSimulator());
		copy->actualState = this->actualState;
		copy->agentsNumber = this->agentsNumber;
		copy->possibleActions = this->possibleActions;
		copy->rewards = this->rewards;
		return copy;
	}

	void MakeAction(MCTestAction act, bool isSimulation) {
		if (find(this->possibleActions.begin(), this->possibleActions.end(), act) == this->possibleActions.end()) {
			throw IllegalOperationException("Wrong action to take!");
		}

		float distance1 = Vec2i::Distancef(actualState.player1Pos, actualState.player2Pos);

		if (act == MCTestAction::GO_DOWN) {
			if (actualState.GetAgentOnTurn() == 0) {
				if (!isSimulation) cout << "Agent 0 going down" << endl;
				actualState.player1Pos.y++;
			}
			else {
				if (!isSimulation) cout << "Agent 1 going down" << endl;
				actualState.player2Pos.y++;
			}
		}
		else if (act == MCTestAction::GO_LEFT) {
			if (actualState.GetAgentOnTurn() == 0) {
				if (!isSimulation) cout << "Agent 0 going left" << endl;
				actualState.player1Pos.x--;
			}
			else {
				if (!isSimulation) cout << "Agent 1 going left" << endl;
				actualState.player2Pos.x--;
			}
		}
		else if (act == MCTestAction::GO_RIGHT) {
			if (actualState.GetAgentOnTurn() == 0) {
				if (!isSimulation) cout << "Agent 0 going right" << endl;
				actualState.player1Pos.x++;
			}
			else {
				if (!isSimulation) cout << "Agent 1 going right" << endl;
				actualState.player2Pos.x++;
			}
		}
		else if (act == MCTestAction::GO_UP) {
			if (actualState.GetAgentOnTurn() == 0) {
				if (!isSimulation) cout << "Agent 0 going up" << endl;
				actualState.player1Pos.y--;
			}
			else {
				if (!isSimulation) cout << "Agent 1 going up" << endl;
				actualState.player2Pos.y--;
			}
		}

		float distance2 = Vec2i::Distancef(actualState.player1Pos, actualState.player2Pos);
		
		if (!isSimulation) cout << "   DISTANCE: " << ((int)distance2) << " ; ["<<actualState.player1Pos.x << "," << actualState.player1Pos.y
			<< "] : [" << actualState.player2Pos.x << "," <<actualState.player2Pos.y << "]" << endl;
		
		int distDiff = (distance2 - distance1);

		// first agent tries to catch the second one
		this->rewards = AgentsReward(distDiff < 0 ? (30-distance2) : 0, 0);

		this->actualState.SwapAgentOnTurn(this->agentsNumber);
		RecalcPossibleActions();
	}

protected:

	virtual void RecalcPossibleActions() {
		this->possibleActions.clear();

		if (Vec2i::Distance(actualState.player1Pos, actualState.player2Pos) == 0) return;

		// all actions are possible
		possibleActions.push_back(MCTestAction::GO_DOWN);
		possibleActions.push_back(MCTestAction::GO_LEFT);
		possibleActions.push_back(MCTestAction::GO_RIGHT);
		possibleActions.push_back(MCTestAction::GO_UP);
	}

};


TEST_CASE("MonteCarloTest test", "[class]")
{

	SECTION("UCT agent tries to catch the random agent")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(120);
		
		// there are two agents, one random and the second one is UCT. 
		// random agent is at location 10,10; UCT agent is at the location 0,0
		// UCT agent has to catch the Random agent; at each step, they can only go in 4 directions
		auto simulator = spt<MCTestSimulator>(new MCTestSimulator());
		auto agents = vector<spt<AIAgent<MCTestState, MCTestAction>>>();
		agents.push_back(spt<UCTAgent<MCTestState, MCTestAction>>(new UCTAgent<MCTestState, MCTestAction>("UCTAgent",64,32,16)));
		agents.push_back(spt<RandomAgent<MCTestState, MCTestAction>>(new RandomAgent<MCTestState, MCTestAction>()));
		auto mt = new MonteCarloSearch<MCTestState, MCTestAction>(simulator, agents);
		mt->RunSearch(1);

		REQUIRE(mt->GetNumberOfWins(0) == 1);
		REQUIRE(mt->GetRewardSum(0) == 419);
	}
}
