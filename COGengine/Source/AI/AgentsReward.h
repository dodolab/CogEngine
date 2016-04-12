#pragma once

#include <vector>
using namespace std;

namespace Cog {

	/**
	* Collection of rewards fo reach game agent
	*/
	class AgentsReward {
	protected:
		// Collection of rewards
		vector<float> rewards;
		// Number of agents
		int agentsNum = 0;
	public:

		// Construct
		AgentsReward() {
			this->agentsNum = 0;
		}

		/**
		* Construct with rewards for two agents
		* @param reward1 reward of the first agent
		* @param reward2 reward of the second agent
		*/
		AgentsReward(float reward1, float reward2) {
			this->agentsNum = 2;
			rewards.push_back(reward1);
			rewards.push_back(reward2);
		}

		/**
		* Construct with rewards for three agents
		* @param reward1 reward of the first agent
		* @param reward2 reward of the second agent
		* @param reward3 reward of the third agent
		*/
		AgentsReward(float reward1, float reward2, float reward3) {
			this->agentsNum = 3;
			rewards.push_back(reward1);
			rewards.push_back(reward2);
			rewards.push_back(reward3);
		}

		/**
		* Construct with rewards for foud agents
		* @param reward1 reward of the first agent
		* @param reward2 reward of the second agent
		* @param reward3 reward of the third agent
		* @param reward4 reward of the fourth agent
		*/
		AgentsReward(float reward1, float reward2, float reward3, float reward4) {
			this->agentsNum = 4;
			rewards.push_back(reward1);
			rewards.push_back(reward2);
			rewards.push_back(reward3);
			rewards.push_back(reward4);
		}

		/**
		* Construct with specifying the number of agents
		* @param agentsNum the number of agents
		*/
		AgentsReward(int agentsNum) : agentsNum(agentsNum) {
			for (int i = 0; i < agentsNum; i++) {
				rewards.push_back(0);
			} 
		}

		/**
		* Gets number of agents
		*/
		int GetAgentsNum() const{
			return agentsNum;
		}

		/**
		* Gets reward of agent at selected index
		* @param agentIndex index of selected agent
		*/
		float GetReward(int agentIndex) const{
			return rewards[agentIndex];
		}

		/**
		* Stores reward of agent at selected index
		* @param agentIndex index of selected agent
		* @param reward reward of the agent
		*/
		void StoreReward(int agentIndex, float reward) {
			rewards[agentIndex] = reward;
		}

		/**
		* Adds a reward value to the agent at selected index
		* @param agentIndex index of the selected agent
		* @param addition reward increment
		*/
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

		/**
		* Gets index of agent that as a highest reward
		* Returns -1, if all agents have the same reward
		*/
		float GetAgentIndexWithHighestReward() const {
			int indexWithHighest = -1;
			float highestVal = -10000;
			int sameValues = 0;
			for (int i = 0; i < agentsNum; i++) {
				if (rewards[i] > highestVal) {
					highestVal = rewards[i];
					indexWithHighest = i;
				}
				else if (rewards[i] == highestVal) {
					sameValues++;
				}
			}

			if (sameValues == agentsNum - 1) return -1;
			else return indexWithHighest;
		}
	};

} // namespace