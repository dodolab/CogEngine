#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"


#include "catch.hpp"
using namespace Cog;

#include "MonteCarloTestAssets.h"






TEST_CASE("MonteCarloTest test")
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

		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);

		TimeMeasure::GetInstance().Report(false);
		// UCT agent won with score 10007
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
		REQUIRE(mt->CalcRewardSum(0) == 10007);
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

		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);

		TimeMeasure::GetInstance().Report(false);
		// UCT agent won with score 10005
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
		REQUIRE(mt->CalcRewardSum(0) == 10005);
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

		
		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		auto sum = mt->CalcRewardSum(1);
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
		REQUIRE(mt->CalcRewardSum(0) == 10007);
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


		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
		REQUIRE(mt->CalcRewardSum(0) == 10026);
	}
}
