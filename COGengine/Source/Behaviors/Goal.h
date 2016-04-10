#pragma once

#include "Behavior.h"

namespace Cog {

	enum class GoalState {
		INACTIVE,
		PROCESSING,
		COMPLETED,
		ABORTED,
		FAILED
	};
	
	/**
	* Common class for goal-driven behavior
	*/
	class Goal : public Behavior {
	protected:
		GoalState goalState = GoalState::INACTIVE;
		StrId stateId;


		virtual void OnGoalComplete() {

		}

		virtual void OnGoalAbort() {

		}

		virtual void OnGoalFail() {

		}

	public:

		Goal() {

		}

		Goal(StrId stateId) : stateId(stateId) {

		}

		inline bool IsInactive() { return goalState == GoalState::INACTIVE; }
		inline bool IsProcessing() { return goalState == GoalState::PROCESSING; }
		inline bool IsCompleted() { return goalState == GoalState::COMPLETED; }
		inline bool IsAborted() { return goalState == GoalState::ABORTED; }
		inline bool IsFailed() { return goalState == GoalState::FAILED; }
		
		inline bool GoalEnded() { return IsCompleted() || IsAborted() || IsFailed(); }

		GoalState GetGoalState() {
			return goalState;
		}

		void SetGoalState(GoalState goalState) {
			this->goalState = goalState;
		}

		void Complete();

		void Fail();

		void Abort();

		StrId GetStateId() {
			return stateId;
		}
	};

	/**
	* Goal composite behavior
	*/
	class GoalComposite : public Goal {
	protected:
		vector<Goal*> subgoals;
		int subgoalIndex = 0;
		Goal* actualSubgoal = nullptr;
		bool continueOnFail = false;
	public:

		GoalComposite(StrId stateId, bool continueOnFail) : Goal(stateId), continueOnFail(continueOnFail){

		}

		void OnStart() {
			if (subgoals.size() > 0) {
				SwitchToSubgoal(subgoals[subgoalIndex]);
			}
		}

		void OnMessage(Msg& msg) {
			if (actualSubgoal != nullptr) {
				actualSubgoal->OnMessage(msg);
			}
		}

		virtual void OnGoalAbort() {
			if (actualSubgoal != nullptr) {
				actualSubgoal->Abort();
			}
		}

		void AddSubgoal(Goal* goal) {
			this->subgoals.push_back(goal);
		}

		vector<Goal*>& GetSubgoals() {
			return subgoals;
		}

		virtual void Update(const uint64 delta, const uint64 absolute);

	protected:
		void SwitchToSubgoal(Goal* goal);
	};

}// namespace