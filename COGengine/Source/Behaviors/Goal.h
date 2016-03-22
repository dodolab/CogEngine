#pragma once

#include "ofxCogCommon.h"
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
		StringHash stateId;


		virtual void OnGoalComplete() {

		}

		virtual void OnGoalAbort() {

		}

		virtual void OnGoalFail() {

		}

	public:

		Goal(StringHash stateId) : stateId(stateId) {

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

		void Complete() {
			SetGoalState(GoalState::COMPLETED);
			OnGoalComplete();
			Finish();
		}

		void Fail() {
			SetGoalState(GoalState::FAILED);
			OnGoalFail();
			Finish();
		}

		void Abort() {
			SetGoalState(GoalState::ABORTED);
			OnGoalAbort();
			Finish();
		}

		StringHash GetStateId() {
			return stateId;
		}
	};

	/**
	* Goal composite behavior
	*/
	class GoalComposite : public Goal {
	protected:
		vector<Goal*> subgoals = vector<Goal*>();
		int subgoalIndex = 0;
		Goal* actualSubgoal = nullptr;
		bool continueOnFail = false;
	public:

		GoalComposite(StringHash stateId, bool continueOnFail) : Goal(stateId), continueOnFail(continueOnFail){

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

		virtual void Update(const uint64 delta, const uint64 absolute) {
			if (actualSubgoal != nullptr) {
				do {
					actualSubgoal->Update(delta, absolute);

					if (actualSubgoal->IsCompleted() || ((actualSubgoal->IsFailed() || actualSubgoal->IsAborted()) && continueOnFail)) {
						if (subgoalIndex < (subgoals.size() - 1)) {
							SwitchToSubgoal(subgoals[++subgoalIndex]);
						}
						else {
							Complete();
							break;
						}
					}
					else if(actualSubgoal->IsFailed() && !continueOnFail) {
						Fail();
						break;
					}
					else {
						break;
					}
				} while (true); // safe
			}
		}

	protected:
		void SwitchToSubgoal(Goal* goal) {
			this->actualSubgoal = goal;
			SetOwner(goal, owner);
			goal->SetGoalState(GoalState::PROCESSING);
			// goal can fail during initialization, so that the state should be changed before init
			goal->Start();
		}
	};

}// namespace