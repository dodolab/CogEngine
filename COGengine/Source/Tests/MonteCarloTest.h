#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;
#include "Vec2i.h"
#include "MonteCarloSearch.h"
#include "RandomAgent.h"
#include "UCTAgent.h"


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
		this->agentOnTurn = copy.agentOnTurn;
		this->marks = copy.marks;
		this->hashCode = copy.hashCode;
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
		// important as well
		this->actualState.SetAgentOnTurn(0);
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

		if (gameOver) totalReward = 10000;

		if (!isSimulation) {
			cout << WriteInfo().c_str() << endl;

		}
		if (actualState.GetAgentOnTurn() == 0) rewards = AgentsReward(totalReward, gameOver ? -1000 : 0);
		else rewards = AgentsReward(gameOver ? -1000 : 0, totalReward);

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

class TicTacToeHumanAgent : public RandomAgent<TicTacToeState, TicTacToeAction> {
protected:
public:

	TicTacToeHumanAgent() {

	}

	TicTacToeHumanAgent(string name) : RandomAgent(name) {

	}

	virtual TicTacToeAction ChooseAction(spt<Simulator<TicTacToeState, TicTacToeAction>> simulator) {
		TicTacToeAction action;
		int mojo = 12;
		int dojo = 13;
		action.positionMark = Vec2i(mojo, dojo);
		return action;
	}

};




TEST_CASE("MonteCarloTest test", "[class]")
{

	SECTION("TIC TAC TOE test for UCT agent and random agent deep search")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(120);

		// Tic tac toe game between UCT agent and random agent
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent",20, sqrt(2)));
		auto agent2 = spt<RandomAgent<TicTacToeState, TicTacToeAction>>(new RandomAgent<TicTacToeState, TicTacToeAction>());
		//auto agent2 = spt<TicTacToeHumanAgent>(new TicTacToeHumanAgent());
		agents.push_back(agent1);
		agents.push_back(agent2);

		auto mt = new MonteCarloSearch<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);

		TimeMeasure::GetInstance().Report(false);
		// UCT agent won with score 10007
		REQUIRE(mt->GetNumberOfWins(0) == 1);
		REQUIRE(mt->GetRewardSum(0) == 10007);
	}

	SECTION("TIC TAC TOE test for UCT agent and random agent shallow search")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(120);

		// Tic tac toe game between UCT agent and random agent
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 200, sqrt(2),5));
		auto agent2 = spt<RandomAgent<TicTacToeState, TicTacToeAction>>(new RandomAgent<TicTacToeState, TicTacToeAction>());
		//auto agent2 = spt<TicTacToeHumanAgent>(new TicTacToeHumanAgent());
		agents.push_back(agent1);
		agents.push_back(agent2);

		auto mt = new MonteCarloSearch<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);

		TimeMeasure::GetInstance().Report(false);
		// UCT agent won with score 10008
		REQUIRE(mt->GetNumberOfWins(0) == 1);
		REQUIRE(mt->GetRewardSum(0) == 10007);
	}

	SECTION("TIC TAC TOE test for two UCT agents deep search")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(125);

		// Tic tac toe game between two uct agents
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 60, sqrt(2)));
		auto agent2 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 60, sqrt(2)));
		agents.push_back(agent1);
		agents.push_back(agent2);

		
		auto mt = new MonteCarloSearch<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		auto sum = mt->GetRewardSum(1);
		REQUIRE(mt->GetNumberOfWins(0) == 1);
		REQUIRE(mt->GetRewardSum(0) == 10007);
	}

	SECTION("TIC TAC TOE test for two UCT agents shallow search")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(125);

		// Tic tac toe game between two uct agents
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 20, sqrt(2),5));
		auto agent2 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 750, sqrt(2),5));
		agents.push_back(agent1);
		agents.push_back(agent2);


		auto mt = new MonteCarloSearch<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		auto sum = mt->GetRewardSum(1);
		REQUIRE(mt->GetNumberOfWins(1) == 1);
		REQUIRE(mt->GetRewardSum(1) == 10007);
	}
}
