#pragma once

#include <vector>
using namespace std;

class AgentsReward {
protected:
	vector<int> rewards;
	int agentsNum = 0;
public:

	AgentsReward() {
		this->agentsNum = 0;
	}

	AgentsReward(int reward1, int reward2) {
		this->agentsNum = 2;
		rewards.push_back(reward1);
		rewards.push_back(reward2);
	}

	AgentsReward(int reward1, int reward2, int reward3) {
		this->agentsNum = 3;
		rewards.push_back(reward1);
		rewards.push_back(reward2);
		rewards.push_back(reward3);
	}

	AgentsReward(int reward1, int reward2, int reward3, int reward4) {
		this->agentsNum = 4;
		rewards.push_back(reward1);
		rewards.push_back(reward2);
		rewards.push_back(reward3);
		rewards.push_back(reward4);
	}

	AgentsReward(int reward1, int reward2, int reward3, int reward4, int reward5) {
		this->agentsNum = 5;
		rewards.push_back(reward1);
		rewards.push_back(reward2);
		rewards.push_back(reward3);
		rewards.push_back(reward4);
		rewards.push_back(reward5);
	}

	AgentsReward(int agentsNum) : agentsNum(agentsNum) {
		for (int i = 0; i < agentsNum; i++) {
			rewards.push_back(0);
		}
	}

	int GetAgentsNum() {
		return agentsNum;
	}

	int GetReward(int agentIndex) {
		return rewards[agentIndex];
	}

	void StoreReward(int agentIndex, int reward) {
		rewards[agentIndex] = reward;
	}

	void AddVal(int agentIndex, int addition) {
		rewards[agentIndex] += addition;
	}

	void Merge(AgentsReward& agRewards) {
		for (int i = 0; i < agentsNum; i++) {
			rewards[i] += agRewards.rewards[i];
		}
	}
};