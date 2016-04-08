#pragma once

#include <vector>
using namespace std;

namespace Cog {

	/**
	* Class that holds rewards for each game agent
	*/
	class AgentsReward {
	protected:
		// collection of rewards
		vector<float> rewards;
		// number of agents
		int agentsNum = 0;
	public:

		AgentsReward() {
			this->agentsNum = 0;
		}

		AgentsReward(float reward1, float reward2) {
			this->agentsNum = 2;
			rewards.push_back(reward1);
			rewards.push_back(reward2);
		}

		AgentsReward(float reward1, float reward2, float reward3) {
			this->agentsNum = 3;
			rewards.push_back(reward1);
			rewards.push_back(reward2);
			rewards.push_back(reward3);
		}

		AgentsReward(float reward1, float reward2, float reward3, float reward4) {
			this->agentsNum = 4;
			rewards.push_back(reward1);
			rewards.push_back(reward2);
			rewards.push_back(reward3);
			rewards.push_back(reward4);
		}

		AgentsReward(float reward1, float reward2, float reward3, float reward4, float reward5) {
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

		float GetReward(int agentIndex) {
			return rewards[agentIndex];
		}

		void StoreReward(int agentIndex, float reward) {
			rewards[agentIndex] = reward;
		}

		void AddVal(int agentIndex, float addition) {
			rewards[agentIndex] += addition;
		}

		/**
		* Merges rewards from another entity
		*/
		void Merge(AgentsReward& agRewards) {
			for (int i = 0; i < agentsNum; i++) {
				rewards[i] += agRewards.rewards[i];
			}
		}
	};

} // namespace