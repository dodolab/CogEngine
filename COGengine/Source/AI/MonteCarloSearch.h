#pragma once

#include <vector>
using namespace std;

#include "ofxSmartPointer.h"

#include "AIState.h"
#include "Simulator.h"
#include "AIAgent.h"
#include "ActionHistory.h"
#include "AgentsReward.h"


template<class S, class A>
class MonteCarloSearch {
protected:
	spt<Simulator<S, A>> mainSimulator;
	vector<spt<AIAgent<S,A>>> agents;
	ActionHistory<S, A> actionHistory;
	vector<AgentsReward> rewards;

public:
	MonteCarloSearch() {

	}

	MonteCarloSearch(spt<Simulator<S, A>> simulator, vector<spt<AIAgent<S,A>>> agents) : mainSimulator(simulator), agents(agents) {

	}

	spt<Simulator<S, A>> GetSimulator() {
		return mainSimulator;
	}

	vector<spt<AIAgent<S,A>>>& GetAgents() {
		return agents;
	}

	ActionHistory<S, A>& GetActionHistory() {
		return actionHistory;
	}

	vector<AgentsReward>& GetRewards() {
		return rewards;
	}

	int GetRewardSum(int agentIndex) {
		int total = 0;
		for (auto& reward : rewards) {
			total += reward.GetReward(agentIndex);
		}
		return total;
	}

	float GetRewardAvg(int agentIndex) {
		return GetRewardSum(agentIndex) / ((float)rewards.size());
	}

	int GetNumberOfWins(int agentIndex) {
		int total = 0;
		for (auto& reward : rewards) {
			
			int agentReward = reward.GetReward(agentIndex);
			bool loose = false;

			for (int i = 0; i < reward.GetAgentsNum(); i++) {
				if (i == agentIndex) continue;
				if (reward.GetReward(i) > agentReward) {
					loose = true;
					break;
				}
			}

			if (!loose) total++;
		}
		return total;
	}

	void RunSearch(int attempts) {
		mainSimulator->InitState();

		for (int i = 0; i < attempts; i++) {
			spt<Simulator<S, A>> simCopy = mainSimulator->DeepCopy();
			RunAttempt(simCopy);
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


	void RunAttempt(spt<Simulator<S,A>> simulator) {
		mainSimulator->InitState();

		while (!mainSimulator->IsDeadEnd()) {
			int agentOnTurn = mainSimulator->GetActualState()->GetAgentOnTurn();

			spt<Simulator<S, A>> simCopy = simulator->DeepCopy();
			simCopy->SetActualState(mainSimulator->GetActualState());
			A action = agents[agentOnTurn]->ChooseAction(simCopy);
			mainSimulator->MakeAction(action);

			rewards.push_back(mainSimulator->GetRewards());

			actionHistory.AddRecord(simulator->GetActualState(), action, agentOnTurn);
		}
	}
};