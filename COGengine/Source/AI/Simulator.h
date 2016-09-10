#pragma once

#include <vector>
using namespace std;

#include "AgentsReward.h"
#include "ofxSmartPointer.h"
#include "StrId.h"

namespace Cog {

	/**
	* AI simulator that makes transition between states according to the selected action,
	* recalculates collection of possible action when the transition is made and stores
	* rewards for each agent
	*/
	template<class S, class A>
	class Simulator {
	protected:
		// actual state
		S actualState;
		// list of possible actions that can be applied on actual state
		vector<A> possibleActions;
		// rewards for each agent that were calculated during the transition of state
		AgentsReward rewards;
		// number of agents in simulator
		int agentsNumber = 0;
		// indicator, if this simulator was passed to agent's ChooseAction method(used for logging)
		bool isInsideAnAgent = false;
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
			// zero rewards
			this->rewards = AgentsReward(agentsNumber);
		}

		/**
		* Sets actual state and list of possible actions with no recalculation
		*/
		void SetActualState(S& state, vector<A>& possibleActions) {
			this->actualState = state;
			this->possibleActions = possibleActions;
			// zero rewards
			this->rewards = AgentsReward(agentsNumber);
		}

		/**
		* Gets indicator whether this simulator was passed to agent's 
		* ChooseAction method (used for logging)
		*/
		bool IsInsideAnAgent() const {
			return isInsideAnAgent;
		}

		/**
		* Sets indicator whether this simulator will be passed to agent's
		* ChooseAction method (used for logging)
		*/
		void SetIsInsideAnAgent(bool isInside) {
			this->isInsideAnAgent = isInside;
		}

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
		bool IsDeadEnd() const {
			return possibleActions.size() == 0;
		}

		/**
		* Gets number of agents
		*/
		int GetAgentsNumber() const {
			return agentsNumber;
		}

		/**
		* Makes a transition according to the selected action
		* @param action action to make
		*/
		virtual void MakeAction(A action) = 0;


	protected:
		/**
		* Recalculates all possible actions
		*/
		virtual void RecalcPossibleActions() = 0;
	};

} // namespace