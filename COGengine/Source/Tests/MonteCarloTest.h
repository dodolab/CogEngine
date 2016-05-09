#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"


#include "catch.hpp"
using namespace Cog;

#include "MonteCarloTestAssets.h"






TEST_CASE("MonteCarloTest test")
{
	SECTION("TIC TAC TOE test for weak UCT agent and best-only agent")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(120);

		// Tic tac toe game between UCT agent and random agent
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 400, sqrt(2),2));
		auto agent2 = spt<BestOnlyAgent<TicTacToeState, TicTacToeAction>>(new BestOnlyAgent<TicTacToeState, TicTacToeAction>());
		agent1->SetActionFilter(spt<TicTacToeActionFilter>(new TicTacToeActionFilter()));
		agents.push_back(agent1);
		agents.push_back(agent2);

		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
 		REQUIRE(mt->CalcNumberOfWins(1) == 1);
	}

	
	SECTION("TIC TAC TOE test for weak UCT agent and random agent")
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

		// UCT agent won with score 10007
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
	}

	SECTION("TIC TAC TOE test for weak UCT agent and random agent")
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

		// UCT agent won with score 10005
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
	}

	SECTION("TIC TAC TOE test for two UCT agents weak vs weak")
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
	}

	SECTION("TIC TAC TOE test for two UCT agents (weak vs strong)")
	{
		// set seed so that the randomness will behave as expected
		ofSeedRandom(125);

		// Tic tac toe game between two uct agents
		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 20, sqrt(2),5)); // 5 iterations limit
		auto agent2 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 1, sqrt(2)));
		agents.push_back(agent1);
		agents.push_back(agent2);


		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);

		// strong UCT agent should win
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
	}

	SECTION("TIC TAC TOE test, BestOnly agent vs RANDOM agent")
	{
		ofSeedRandom(125);

		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<BestOnlyAgent<TicTacToeState, TicTacToeAction>>(new BestOnlyAgent<TicTacToeState, TicTacToeAction>("BestOnlyAgent"));
		auto agent2 = spt<RandomAgent<TicTacToeState, TicTacToeAction>>(new RandomAgent<TicTacToeState, TicTacToeAction>("RandomAgent"));
		agents.push_back(agent1);
		agents.push_back(agent2);


		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		// prior agent should win
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
	}

	SECTION("TIC TAC TOE test, BestOnlyAgent vs weak UCT agent")
	{
		ofSeedRandom(125);

		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<BestOnlyAgent<TicTacToeState, TicTacToeAction>>(new BestOnlyAgent<TicTacToeState, TicTacToeAction>("BestOnlyAgent"));
		// 5 simulations makes UCT agent to play pseudo-randomly
		auto agent2 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 5, sqrt(2)));

		agents.push_back(agent1);
		agents.push_back(agent2);


		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		// prior agent should win with little effort
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
	}

	SECTION("TIC TAC TOE test, BestOnlyAgent vs weak UCT agent")
	{
		ofSeedRandom(125);

		auto simulator = spt<TicTacToeSimulator>(new TicTacToeSimulator());
		auto agents = vector<spt<AIAgent<TicTacToeState, TicTacToeAction>>>();
		auto agent1 = spt<BestOnlyAgent<TicTacToeState, TicTacToeAction>>(new BestOnlyAgent<TicTacToeState, TicTacToeAction>("BestOnlyAgent"));
		auto agent2 = spt<UCTAgent<TicTacToeState, TicTacToeAction>>(new UCTAgent<TicTacToeState, TicTacToeAction>("UCTAgent", 1, sqrt(2)));

		agents.push_back(agent1);
		agents.push_back(agent2);


		auto mt = new GameSimulator<TicTacToeState, TicTacToeAction>(simulator, agents);
		mt->RunSimulations(1);
		
		REQUIRE(mt->CalcNumberOfWins(0) == 1);
	}
}
