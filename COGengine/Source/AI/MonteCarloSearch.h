#pragma once

#include <vector>
using namespace std;

#include "AIState.h"
#include "Simulator.h"
#include "AIAgent.h"
#include "ActionHistory.h"
#include "AgentsReward.h"

namespace Cog {

	/**
	* MonteCarlo searching algorithm
	* Uses simulator and agents to simulate game and collects
	* statistical data
	*/
	template<class S, class A>
	class MonteCarloSearch {
	protected:
		spt<Simulator<S, A>> mainSimulator;
		vector<spt<AIAgent<S, A>>> agents;
		ActionHistory<S, A> actionHistory;
		// sum of rewards for each agent
		vector<AgentsReward> totalRewards;
		// holds number of steps for each simulation
		vector<int> stepsCounter;

	public:
		MonteCarloSearch() {

		}

		MonteCarloSearch(spt<Simulator<S,A>> simulator, vector<spt<AIAgent<S,A>>> agents) : mainSimulator(simulator), agents(agents) {
		}

		spt<Simulator<S,A>> GetSimulator() {
			return mainSimulator;
		}

		vector<spt<AIAgent<S,A>>>& GetAgents() {
			return agents;
		}

		ActionHistory<S, A>& GetActionHistory() {
			return actionHistory;
		}

		/**
		* Calculates sum of rewards for selected agent
		*/
		int GetRewardSum(int agentIndex) {
			int total = 0;
			for (auto& reward : totalRewards) {
				total += reward.GetReward(agentIndex);
			}
			return total;
		}

		/**
		* Calculates avg reward for selected agent
		*/
		float GetRewardAvg(int agentIndex) {
			return GetRewardSum(agentIndex) / ((float)GetRewardsNum());
		}

		/**
		* Calculates number of all steps during the simulation
		*/
		int GetStepsSum() {
			int total = 0;
			for (auto& step : stepsCounter) {
				total += step;
			}
			return total;
		}

		/**
		* Calculates number of wins for selected agent
		*/
		int GetNumberOfWins(int agentIndex) {
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

		void RunSimulations(int attempts, bool initState = true) {

			// run simulation
			for (int i = 0; i < attempts; i++) {
				RunSimulation(initState);
			}

			COGLOGDEBUG("AI", WriteInfo().c_str());
		}

	protected:

		/**
		* Executes simulation for one attempt
		*/
		void RunSimulation(bool initState) {

			auto loopSimulator = mainSimulator->DeepCopy(); // simulator for this loop
			auto agentSimulator = mainSimulator->DeepCopy(); // simulator for agents (let them do it with it what they want)

			if(initState) loopSimulator->InitState();
			
			int stepCounter = 0;
			AgentsReward rw = AgentsReward(agents.size());

			while (!loopSimulator->IsDeadEnd()) {
				// get agent on turn
				int agentOnTurn = loopSimulator->GetActualState().GetAgentOnTurn();

				// copy state of main simulator into its copy
				agentSimulator->SetActualState(loopSimulator->GetActualState());
				auto agent = agents[agentOnTurn];
				// let the agent choose the appropriate action
				A action = agent->ChooseAction(agentSimulator);

				// apply the selected action into the simulator
				loopSimulator->MakeAction(action,false);
				// get rewards of the selected action
				stepCounter++;
				rw.Merge(loopSimulator->GetRewards());
				// update history
				actionHistory.AddRecord(loopSimulator->GetActualState(), action, agentOnTurn);
			}

			this->totalRewards.push_back(rw);
			this->stepsCounter.push_back(stepCounter);
		}

		string WriteInfo() {

			ostringstream ss;

			ss << "MonteCarloSearch result:" << endl;

			ss << "Rewards: [ ";

			AgentsReward& rewards = this->mainSimulator->GetRewards();
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				ss << rewards.GetReward(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			ss << endl;

			ss << "RewardsSum: [ ";
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				ss << GetRewardSum(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			ss << endl;

			ss << "Wins: [ ";
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				ss << GetNumberOfWins(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			ss << endl;
			return ss.str();
		}
	};

} // namespace