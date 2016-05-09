#pragma once

#include <vector>
using namespace std;

#include "AgentsReward.h"
#include "Definitions.h"
#include "StrId.h"
#include "ActionFilter.h"

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
		// filter that may be used to limit collection of possible actions
		spt<ActionFilter<S, A>> actionFilter;
	public:

		Simulator() {
			actionFilter = spt<ActionFilter<S, A>>();
		}

		/**
		* Initializes the simulator
		*/
		virtual void InitState() = 0;

		/**
		* Makes a deep copy of the simulator
		*/
		virtual spt<Simulator> DeepCopy() {
			auto copy = DeepCopyImpl();
			copy->actualState = this->actualState;
			copy->agentsNumber = this->agentsNumber;
			copy->possibleActions = this->possibleActions;
			copy->rewards = this->rewards;
			copy->isInsideAnAgent = this->isInsideAnAgent;
			copy->actionFilter = this->actionFilter;
			return copy;
		}

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
		* Gets action filter
		*/
		spt<ActionFilter<S, A>> GetActionFilter() {
			return actionFilter;
		}

		/**
		* Sets action filter
		*/
		void SetActionFilter(spt<ActionFilter<S, A>> filter) {
			this->actionFilter = filter;
		}

		/**
		* Makes a transition according to the selected action
		* @param action action to make
		*/
		void MakeAction(A action) {
			MakeActionImpl(action);
			this->actualState.SwapAgentOnTurn(this->agentsNumber);
			RecalcPossibleActions();
		}

	protected:
		
		virtual spt<Simulator> DeepCopyImpl() = 0;

		virtual void MakeActionImpl(A action) = 0;


		/**
		* Recalculates all possible actions
		*/
		virtual void RecalcPossibleActionsImpl() = 0;

		void RecalcPossibleActions() {
			this->possibleActions.clear();

			RecalcPossibleActionsImpl();
			if (actionFilter) actionFilter->ApplyFilter(actualState, possibleActions);
		}
	};

} // namespace