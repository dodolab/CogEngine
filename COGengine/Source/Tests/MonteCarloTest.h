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
				actualState.player1Pos.y++;
			}
			else {
				actualState.player2Pos.y++;
			}
		}
		else if (act == MCTestAction::GO_LEFT) {
			if (actualState.GetAgentOnTurn() == 0) {
				actualState.player1Pos.x--;
			}
			else {
				actualState.player2Pos.x--;
			}
		}
		else if (act == MCTestAction::GO_RIGHT) {
			if (actualState.GetAgentOnTurn() == 0) {
				actualState.player1Pos.x++;
			}
			else {
				actualState.player2Pos.x++;
			}
		}
		else if (act == MCTestAction::GO_UP) {
			if (actualState.GetAgentOnTurn() == 0) {
				actualState.player1Pos.y--;
			}
			else {
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

// =========================================== TIC TAC TOE =============================================

class TicTacToeAction {
public:
	Vec2i positionMark;

	inline bool operator==(const TicTacToeAction& a) const {
		return positionMark == a.positionMark;
	}
	inline bool operator!=(const TicTacToeAction& a) const { return !(*this == a); }
};

enum class TTMark {
	AI = 1, 
	PLAYER = 2, 
	EMPTY = 3
};

class TicTacToeState : public AIState {
public:
	map<Vec2i, TTMark> marks;

	TicTacToeState() {

	}

	TicTacToeState(int agentOnTurn) {
		this->agentOnTurn = agentOnTurn;
	}

	TicTacToeState(const TicTacToeState& copy) {
		this->agentOnTurn = agentOnTurn;
		this->marks = copy.marks;
	}

	TTMark GetMark(Vec2i point) const {
		return (*marks.find(point)).second;
	}

	inline bool operator==(const TicTacToeState& a) const {
		for (auto mark : marks) {
			if (a.GetMark(mark.first) != mark.second) return false;
		}
		return true;
	}
	inline bool operator!=(const TicTacToeState& a) const { return !(*this == a); }
};


class TicTacToeSimulator : public Simulator<TicTacToeState, TicTacToeAction>
{
public:

	TicTacToeSimulator() {
		this->agentsNumber = 2;
	}

	void InitState() {
		this->actualState = TicTacToeState();

		// board 8x8
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				actualState.marks[Vec2i(i, j)] = TTMark::EMPTY;
			}
		}
		this->rewards = AgentsReward(0, 0);
		RecalcPossibleActions();
	}

	spt<Simulator> DeepCopy() {
		auto copy = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		copy->actualState = this->actualState;
		copy->agentsNumber = this->agentsNumber;
		copy->possibleActions = this->possibleActions;
		copy->rewards = this->rewards;
		return copy;
	}

	bool gameOver = false;

	void MakeAction(TicTacToeAction act, bool isSimulation) {
		if (find(this->possibleActions.begin(), this->possibleActions.end(), act) == this->possibleActions.end()) {
			throw IllegalOperationException("Wrong action to take!");
		}

		gameOver = false;

		TTMark mark = actualState.GetAgentOnTurn() == 0 ? TTMark::AI : TTMark::PLAYER;

		actualState.marks[act.positionMark] = mark;

		// check row left and right
		int startX = act.positionMark.x;
		int startY = act.positionMark.y;
		auto& marks = actualState.marks;

		int totalReward = 0;

		// ============================ check row ========================
		int pivot = startX-1;
		int counter = 1;
		
		while (pivot >= 0) {
			if (marks[Vec2i(pivot, startY)] != mark) break;
			else {
				counter++;
				pivot--;
			}
		}

		pivot = startX + 1;
		while (pivot < 8) {
			if (marks[Vec2i(pivot, startY)] != mark) break;
			else {
				counter++;
				pivot++;
			}
		}

		if (counter >= 5) gameOver = true;
		totalReward += (counter-1);

		// ============================ check column ========================
		counter = 1;
		pivot = startY - 1;

		while (pivot >= 0) {
			if (marks[Vec2i(startX, pivot)] != mark) break;
			else {
				counter++;
				pivot--;
			}
		}

		pivot = startY + 1;
		while (pivot < 8) {
			if (marks[Vec2i(startX, pivot)] != mark) break;
			else {
				counter++;
				pivot++;
			}
		}

		if (counter >= 5) gameOver = true;
		totalReward += (counter-1);

		// ============================ check first diag ========================
		counter = 1;
		int pivotX = startX - 1;
		int pivotY = startY - 1;

		while (pivotX >= 0 && pivotY >= 0) {
			if (marks[Vec2i(pivotX, pivotY)] != mark) break;
			else {
				counter++;
				pivotX--;
				pivotY--;
			}
		}

		pivotX = startX + 1;
		pivotY = startY + 1;
		
		while (pivotX < 8 && pivotY < 8) {
			if (marks[Vec2i(pivotX, pivotY)] != mark) break;
			else {
				counter++;
				pivotX++;
				pivotY++;
			}
		}

		if (counter >= 5) gameOver = true;
		totalReward += (counter-1);

		// ============================ check second diag ========================
		counter = 1;
		pivotX = startX + 1;
		pivotY = startY - 1;

		while (pivotX < 8 && pivotY >= 0) {
			if (marks[Vec2i(pivotX, pivotY)] != mark) break;
			else {
				counter++;
				pivotX++;
				pivotY--;
			}
		}

		pivot = startX - 1;
		pivot = startY + 1;
		while (pivotX >= 0 && pivotY < 8) {
			if (marks[Vec2i(pivotX, pivotY)] != mark) break;
			else {
				counter++;
				pivotX--;
				pivotY++;
			}
		}

		if (counter >= 5) gameOver = true;
		totalReward += (counter - 1);

		if (gameOver) totalReward = 100;

		if (!isSimulation) {
			cout << WriteInfo().c_str() << endl;
			bool mojo = false;
		}
		if (actualState.GetAgentOnTurn() == 0) rewards = AgentsReward(totalReward, 0);
		else rewards = AgentsReward(0, totalReward);

		this->actualState.SwapAgentOnTurn(this->agentsNumber);
		RecalcPossibleActions();
	}

protected:
	
	string WriteInfo() {
		auto& markMap = actualState.marks;
		ostringstream ss;
		ss << "Tic tac toe result (UCT is x)" << endl;
		for (int i = 0; i < 8; i++) {
			ss << "|";
			for (int j = 0; j < 8; j++) {
				auto mark = markMap[Vec2i(j, i)];
				if (mark == TTMark::AI) ss << "x";
				else if (mark == TTMark::EMPTY) ss << " ";
				else if (mark == TTMark::PLAYER) ss << "o";
				ss << "|";
				
			}
			ss << endl;
		}
		return ss.str();
	}

	virtual void RecalcPossibleActions() {
		this->possibleActions.clear();

		if (gameOver) return;
		auto& marks = actualState.marks;

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (marks[Vec2i(i, j)] == TTMark::EMPTY) {
					auto action = TicTacToeAction();
					action.positionMark = Vec2i(i, j);
					this->possibleActions.push_back(action);
				}
			}
		}
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
		mt->RunSimulations(1);

		REQUIRE(mt->GetNumberOfWins(0) == 1);
		REQUIRE(mt->GetRewardSum(0) == 419);
	}

	SECTION("TIC TAC TOE test for UCT agent")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(120);

		// there are two agents, one random and the second one is UCT. 
		// both play tic tac toe
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent",8, sqrt(2),10));
		agent->SetNumberOfUCTTrees(5);
		agents.push_back(agent);
		agents.push_back(spt<RandomAgent<TicTacToeState, TicTacToeAction>>(new RandomAgent<TicTacToeState, TicTacToeAction>()));
		auto mt = new MonteCarloSearch<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);

		REQUIRE(mt->GetNumberOfWins(0) == 1);
		REQUIRE(mt->GetRewardSum(0) == 419);
	}
}
