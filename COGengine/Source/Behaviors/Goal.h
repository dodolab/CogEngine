#pragma once

#include "ofxCogCommon.h"
#include "Behavior.h"

namespace Cog {

	enum class GoalState {
		INACTIVE,
		PROCESSING,
		COMPLETED,
		FAILED
	};
	
	/**
	* Common class for goal-driven behavior
	*/
	class Goal : public Behavior {
	protected:
		GoalState goalState = GoalState::INACTIVE;
	public:

		inline bool IsInactive() { return goalState == GoalState::INACTIVE; }
		inline bool IsProcessing() { return goalState == GoalState::PROCESSING; }
		inline bool IsCompleted() { return goalState == GoalState::COMPLETED; }
		inline bool IsFailed() { return goalState == GoalState::FAILED; }

		GoalState GetGoalState() {
			return goalState;
		}

		void SetGoalState(GoalState goalState) {
			this->goalState = goalState;
		}

		void Complete() {
			SetGoalState(GoalState::COMPLETED);
		}

		void Fail() {
			SetGoalState(GoalState::FAILED);
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

		GoalComposite(bool continueOnFail) : continueOnFail(continueOnFail){

		}

		void Init() {
			if (subgoals.size() > 0) {
				actualSubgoal = subgoals[subgoalIndex];
				actualSubgoal->Init();
			}
		}

		void OnMessage(Msg& msg) {
			if (actualSubgoal != nullptr) {
				actualSubgoal->OnMessage(msg);
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

					if (actualSubgoal->IsCompleted() || (actualSubgoal->IsFailed() && continueOnFail)) {
						if (subgoalIndex < (subgoals.size() - 1)) {
							actualSubgoal = subgoals[++subgoalIndex];
							actualSubgoal->Init();
							actualSubgoal->SetGoalState(GoalState::PROCESSING);
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
	};

}// namespace