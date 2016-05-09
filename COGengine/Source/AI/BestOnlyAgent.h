#pragma once

#include "AIAgent.h"
#include "ofMath.h"

namespace Cog {

	/**
	* Agent that selects always the action with highest reward
	*/
	template<class S, class A>
	class BestOnlyAgent : public AIAgent<S,A> {
	protected:
	public:

		BestOnlyAgent() {

		}

		BestOnlyAgent(string name) : AIAgent<S,A>(name) {

		}

		virtual A ChooseAction(spt<Simulator<S,A>> simulator) {
			auto& actions = simulator->GetPossibleActions();
			
			int actualAgent = simulator->GetActualState().GetAgentOnTurn();
			A bestAction;
			int bestReward = -1000;

			// for each action, get a reward
			for (auto action : actions) {
				auto copy = simulator->DeepCopy();
				copy->MakeAction(action);
				AgentsReward& rew = copy->GetRewards();
				int reward = rew.GetReward(actualAgent);
				if (reward > bestReward) {
					bestReward = reward;
					bestAction = action;
				}
			}

			return bestAction;
		}
	};

} // namespace