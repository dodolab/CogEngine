#pragma once

#include <vector>
using namespace std;

#include "AgentsReward.h"
#include "ofxSmartPointer.h"
#include "StringHash.h"

namespace Cog {

	template<class S, class A>
	class Simulator {
	protected:
		S actualState;
		vector<A> possibleActions;
		AgentsReward rewards;
		int agentsNumber;

	public:

		Simulator() {

		}

		virtual void InitState() = 0;

		S& GetActualState() {
			return actualState;
		}

		virtual spt<Simulator> DeepCopy() = 0;

		void SetActualState(S state) {
			this->actualState = state;
			RecalcPossibleActions();
		}

		void SetActualState(S state, vector<A> possibleActions) {
			this->actualState = state;
			this->possibleActions = possibleActions;
			this->rewards = AgentsReward(agentsNumber);
		}

		virtual void MakeAction(A action, bool isSimulation = true) = 0;

		vector<A>& GetPossibleActions() {
			return possibleActions;
		}

		AgentsReward& GetRewards() {
			return rewards;
		}


		bool IsDeadEnd() {
			return possibleActions.size() == 0;
		}

		int GetAgentsNumber() {
			return agentsNumber;
		}

	protected:
		virtual void RecalcPossibleActions() = 0;
	};

} // namespace