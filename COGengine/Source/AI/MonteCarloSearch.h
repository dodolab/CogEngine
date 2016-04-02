#pragma once

#include <vector>
using namespace std;

#include "AIState.h"
#include "Simulator.h"
#include "AIAgent.h"
#include "ActionHistory.h"
#include "AgentsReward.h"

namespace Cog {

	template<class S, class A>
	class MonteCarloSearch {
	protected:
		spt<Simulator<S, A>> mainSimulator;
		vector<spt<AIAgent<S, A>>> agents;
		ActionHistory<S, A> actionHistory;
		vector<AgentsReward> totalRewards;
		vector<int> rewardsCounter;

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

		int GetRewardSum(int agentIndex) {
			int total = 0;
			for (auto& reward : totalRewards) {
				total += reward.GetReward(agentIndex);
			}
			return total;
		}

		float GetRewardAvg(int agentIndex) {
			return GetRewardSum(agentIndex) / ((float)GetRewardsNum());
		}

		int GetRewardsNum() {
			int total = 0;
			for (auto& rCounter : rewardsCounter) {
				total += rCounter;
			}
			return total;
		}

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

		void RunSearch(int attempts) {

			for (int i = 0; i < attempts; i++) {
				RunSimulation();
			}

			cout << "remaining legal actions: " << this->mainSimulator->GetPossibleActions().size() << endl;
			cout << "Rewards: [ ";

			AgentsReward& rewards = this->mainSimulator->GetRewards();
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				cout << rewards.GetReward(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			cout << endl;

			cout << "RewardsSum: [ ";
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				cout << GetRewardSum(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			cout << endl;

			cout << "Wins: [ ";
			for (int i = 0; i < rewards.GetAgentsNum(); i++) {
				cout << GetNumberOfWins(i) << (i == (rewards.GetAgentsNum() - 1) ? " ]" : ", ");
			}

			cout << endl;
		}

	protected:


		void RunSimulation() {

			// initialize main simulator at the beginning of each loop
			mainSimulator->InitState();
			int rewardsCount = 0;
			AgentsReward rw = AgentsReward(agents.size());

			// this simulator can be used by agent for additional search
			spt<Simulator<S,A>> simCopy = mainSimulator->DeepCopy();

			while (!mainSimulator->IsDeadEnd()) {
				// get agent on turn
				int agentOnTurn = mainSimulator->GetActualState().GetAgentOnTurn();

				// copy state of main simulator into its copy
				simCopy->SetActualState(mainSimulator->GetActualState());
				auto agent = agents[agentOnTurn];
				// let the agent choose the appropriate action
				A action = agent->ChooseAction(simCopy);

				// apply the selected action into the simulator
				mainSimulator->MakeAction(action,false);
				// get rewards of the selected action
				rewardsCount++;
				rw.Merge(mainSimulator->GetRewards());
				// update history
				actionHistory.AddRecord(mainSimulator->GetActualState(), action, agentOnTurn);
			}

			this->totalRewards.push_back(rw);
			this->rewardsCounter.push_back(rewardsCount);
		}
	};

} // namespace