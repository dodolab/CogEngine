#pragma once

#include "Behavior.h"

namespace Cog {

	/**
	* States of the goal
	*/
	enum class GoalState {
		INACTIVE,		/** not active but still ready to run */
		PROCESSING,		/** processing/running */
		COMPLETED,		/** complted/finished */
		ABORTED,		/** aborted */
		FAILED			/** failed */
	};

	/**
	* Type of composite goal
	*/
	enum class GoalCompositeType {
		SERIALIZER,		/** Finishes when all subgoals ended (either failed or completed) */
		SELECTOR,		/** Finishes when first subgoal completes */
		SEQUENCER		/** Finishes when first subgoal fails*/
	};
	
	/**
	* Base class for goal-driven behavior
	* Goals are similar to finite state machine but they are more hierarchical;
	* Each goal can have its subgoals and therefore there may be goals with any level of complexity
	* While a state is static, goals are dynamic and make sense until they are finished
	*
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

		GoalState GetGoalState() const {
			return goalState;
		}

		void SetGoalState(GoalState goalState) {
			this->goalState = goalState;
		}

		/**
		* Completes the goal
		*/
		void Complete();

		/**
		* Fails the goal
		*/
		void Fail();

		/**
		* Aborts the goal
		*/
		void Abort();

		StrId GetStateId() const {
			return stateId;
		}
	};

	/**
	* Goal composite behavior
	*/
	class GoalComposite : public Goal {
	protected:
		GoalCompositeType compositeType;
		vector<Goal*> subgoals;
		int subgoalIndex = 0;
		Goal* actualSubgoal = nullptr;
	public:

		/**
		* Creates a new composite goal
		* @param goalId identifier of the goal
		* @param compositeType type of the composite
		*/
		GoalComposite(StrId goalId, GoalCompositeType compositeType) 
			: Goal(goalId), compositeType(compositeType){

		}

		void OnStart() {
			// switch to first subgoal
			if (subgoals.size() > 0) {
				SwitchToSubgoal(subgoals[subgoalIndex]);
			}
		}

		void OnMessage(Msg& msg) {
			// send message to actual subgoal
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
		/**
		* Switches to another subgoal
		*/
		void SwitchToSubgoal(Goal* goal);
	};

}// namespace