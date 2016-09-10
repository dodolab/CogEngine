#pragma once

#include <vector>
using namespace std;

namespace Cog {

	template<class S, class A>
	class ActionHistory {
	protected:
		class ActionRecord {
		protected:
			S state;
			A action;

		public:
			ActionRecord(S state, A action) {
				this->state = state;
				this->action = action;
			}

			S GetState() {
				return state;
			}

			A GetAction() {
				return action;
			}
		};

		map<int, vector<ActionRecord>> records;
		int recordsNum = 0;
	public:
		void AddRecord(S state, A action, int agentIndex) {

			if (records.find(agentIndex) == records.end()) records[agentIndex] = vector<ActionRecord>();

			auto& agentRecords = records[agentIndex];
			agentRecords.push_back(ActionRecord(state, action));
			recordsNum++;
		}

		S GetState(int agentIndex, int recordIndex) {
			return records[agentIndex][recordIndex].GetState();
		}

		A& GetAction(int agentIndex, int recordIndex) {
			return records[agentIndex][recordIndex].GetAction();
		}

		int GetRecordsNum() {
			return recordsNum;
		}
	};

} // namespace