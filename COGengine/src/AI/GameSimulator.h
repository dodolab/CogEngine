#pragma once

#include <vector>
using namespace std;

#include "AIState.h"
#include "Simulator.h"
#include "AIAgent.h"
#include "AgentsReward.h"

namespace Cog {

	/**
	* Game simulator - simulates game between selected agents and collects results
	* To simulate a game, three entities must be defined:
	* - state that represents any state of a game, derived from AIState class
	* - action that represents transition between two states (equality operators must be overridden)
	* - simulator that makes a transition according to the selected action, calculates collection of possible actions and rewards for each agent
	* @tparam S state the simulator operates with
	* @tparam A action the simulator and the state operate with
	*/
	template<class S, class A>
	class GameSimulator {
	protected:
		// main simulator whose clone is used during the loop
		spt<Simulator<S, A>> mainSimulator;
		// collection of agents
		vector<spt<AIAgent<S, A>>> agents;
		// sum of rewards for each agent per each simulation
		vector<AgentsReward> totalRewards;
		// number of steps for each simulation
		vector<int> stepsCounter;

	public:

		GameSimulator() {

		}

		/**
		* Creates a new Game Simulator
		* @param simulator main simulator that makes transition between states
		* @param agents collection of agents that will play the game
		*/
		GameSimulator(spt<Simulator<S,A>> simulator, vector<spt<AIAgent<S,A>>> agents)
			: mainSimulator(simulator), agents(agents) {
		}

		/**
		* Gets the main simulator
		*/
		spt<Simulator<S,A>> GetSimulator() const{
			return mainSimulator;
		}

		/**
		* Gets the collection of agents
		*/
		vector<spt<AIAgent<S,A>>>& GetAgents() const{
			return agents;
		}

		/**
		* Calculates sum of rewards for selected agent
		*/
		int CalcRewardSum(int agentIndex) const{
			int total = 0;
			for (auto& reward : totalRewards) {
				total += reward.GetReward(agentIndex);
			}
			return total;
		}

		/**
		* Calculates average reward for selected agent
		*/
		float CalcRewardAvg(int agentIndex) const{
			return GetRewardSum(agentIndex) / ((float)GetRewardsNum());
		}

		/**
		* Calculates number of all steps during all simulations
		*/
		int CalcStepsSum() const{
			int total = 0;
			for (auto& step : stepsCounter) {
				total += step;
			}
			return total;
		}

		/**
		* Calculates number of wins for selected agent
		*/
		int CalcNumberOfWins(int agentIndex) const {
			int total = 0;

			for (auto& reward : totalRewards) {
				int winningAgent = 0;

				for (int i = 0; i < agents.size(); i++) {
					if (reward.GetReward(i) > reward.GetReward(winningAgent)) {
						winningAgent = i;
					}
				}

				if (winningAgent == agentIndex && reward.GetReward(winningAgent) > 0) total++;
			}
			return total;
		}

		/**
		* Executes simulations
		* @param attempts number of simulations
		* @param initState indicator, if the simulator should be reinitialized
		*/
		void RunSimulations(int attempts, bool initState = true) {
			// run simulation
			for (int i = 0; i < attempts; i++) {
				RunSimulation(initState);
			}
		}

	protected:

		/**
		* Executes one simulation
		* @param initState indicator, if the simulator should be reinitialized
		*/
		void RunSimulation(bool initState) {
			// simulator for this loop
			auto loopSimulator = mainSimulator->DeepCopy(); 
			// simulator for agents (they can play with it as they please)
			auto agentSimulator = mainSimulator->DeepCopy();
			agentSimulator->SetIsInsideAnAgent(true);

			// initialize simulator
			if (initState) loopSimulator->InitState();

			int stepCounter = 0;
			AgentsReward rw = AgentsReward(agents.size());

			// simulate until the game has ended
			while (!loopSimulator->IsDeadEnd()) {

				int agentOnTurn = loopSimulator->GetActualState().GetAgentOnTurn();

				// copy state of main simulator into its copy
				agentSimulator->SetActualState(loopSimulator->GetActualState());
				
				auto agent = agents[agentOnTurn];
				
				// let the agent choose the appropriate action
				A action = agent->ChooseAction(agentSimulator);

				// apply the selected action into the simulator
				loopSimulator->MakeAction(action);
				
				stepCounter++;
				// merge rewards of the selected action
				rw.Merge(loopSimulator->GetRewards());
			}

			this->totalRewards.push_back(rw);
			this->stepsCounter.push_back(stepCounter);
		}

		/**
		* Writes info about collected data into string
		*/
		string WriteInfo() const {
			ostringstream ss;

			ss << "GameSimulator result:" << endl;

			ss << "Rewards: [ ";

			AgentsReward& rewards = this->mainSimulator->GetRewards();
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				ss << rewards.GetReward(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			ss << endl;

			ss << "RewardsSum: [ ";
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				ss << CalcRewardSum(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			ss << endl;

			ss << "Wins: [ ";
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				ss << CalcNumberOfWins(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			ss << endl;
			return ss.str();
		}
	};

} // namespace