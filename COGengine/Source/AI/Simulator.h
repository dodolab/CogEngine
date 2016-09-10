#pragma once

#include <vector>
using namespace std;

#include "AgentsReward.h"
#include "ofxSmartPointer.h"
#include "StringHash.h"

namespace Cog {

	/**
	* AI simulator that holds state and transforms it, based on selected action;
	* When a transformation is made, collection of possible actions is recalculated
	* and the reward for each agent is saved
	*/
	template<class S, class A>
	class Simulator {
	protected:
		// actual state
		S actualState;
		// list of possible actions that can be applied on actual state
		vector<A> possibleActions;
		// rewards for each agent that were calculated during the transformation of state
		AgentsReward rewards;
		// number of agents in simulator
		int agentsNumber;

	public:

		Simulator() {

		}

		/**
		* Initializes the simulator
		*/
		virtual void InitState() = 0;

		/**
		* Makes a deep copy of the simulator
		*/
		virtual spt<Simulator> DeepCopy() = 0;

		/**
		* Gets the actual state
		*/
		S& GetActualState() {
			return actualState;
		}

		/**
		* Sets actual state and recalculates list of possible actions
		*/
		void SetActualState(S& state) {
			this->actualState = state;
			RecalcPossibleActions();
			this->rewards = AgentsReward(agentsNumber);
		}

		/**
		* Sets actual state and list of possible actions with no recalculation
		*/
		void SetActualState(S& state, vector<A>& possibleActions) {
			this->actualState = state;
			this->possibleActions = possibleActions;
			this->rewards = AgentsReward(agentsNumber);
		}

		/**
		* Transforms the current state to another state based on selected action
		* @param action action to make
		* @param isSimulation if true, the simulator is part of another simulator; used for logging
		*/
		virtual void MakeAction(A action, bool isSimulation = true) = 0;

		/**
		* Gets list of all actions that can be applied to the current state
		*/
		vector<A>& GetPossibleActions() {
			return possibleActions;
		}

		/**
		* Gets rewards for each agent from previous transformation
		*/
		AgentsReward& GetRewards() {
			return rewards;
		}

		/**
		* Returns true, if there is no action to make
		*/
		bool IsDeadEnd() {
			return possibleActions.size() == 0;
		}

		/**
		* Gets number of agents
		*/
		int GetAgentsNumber() {
			return agentsNumber;
		}

	protected:
		/**
		* Recalculates all possible actions
		*/
		virtual void RecalcPossibleActions() = 0;
	};

} // namespace