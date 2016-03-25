
#include "Goal.h"
#include "CogEngine.h"

namespace Cog {

	void Goal::Complete() {
		SetGoalState(GoalState::COMPLETED);
		OnGoalComplete();
		Finish();
	}

	void Goal::Fail() {
		SetGoalState(GoalState::FAILED);
		OnGoalFail();
		Finish();
	}

	void Goal::Abort() {
		SetGoalState(GoalState::ABORTED);
		OnGoalAbort();
		Finish();
	}


	void GoalComposite::Update(const uint64 delta, const uint64 absolute) {
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
				else if (actualSubgoal->IsFailed() && !continueOnFail) {
					Fail();
					break;
				}
				else {
					break;
				}
			} while (true); // safe
		}
	}

	void GoalComposite::SwitchToSubgoal(Goal* goal) {
		this->actualSubgoal = goal;
		SetOwner(goal, owner);
		goal->SetGoalState(GoalState::PROCESSING);
		// goal can fail during initialization, so that the state should be changed before init
		goal->Start();
	}

}// namespace