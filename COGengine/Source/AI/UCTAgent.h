#pragma once

#include <vector>

#include "StringHash.h"
#include "AIAgent.h"
#include "ofMath.h"
#include "ofxSmartPointer.h"
#include "RandomAgent.h"
#include "AgentsReward.h"
#include "Definitions.h"

namespace Cog {

	enum class UCTCalcMethod {
		HIGHEST_REWARD,
		BEST_AVG_REWARD
	};

	template<class S, class A>
	class UCTAgent : public AIAgent<S, A> {
	protected:
		// number of simulations
		int numberOfSims;
		// uct parameter; sets balance between exploration and exploitation
		double uctParameter;
		
		// max state sample to check for each action; -1 for infinite
		int stateBranchSize = -1;
		
		// number of UCT trees for evaluation
		int numberOfUCTTrees = 1;
		// iteration limit for separate game simulation
		int iterationLimit = -1;
		// base agent that is used for random play (usually random agent)
		spt<AIAgent<S, A>> baseAgent;

		UCTCalcMethod calcMethod = UCTCalcMethod::HIGHEST_REWARD;

	public:
		int GetIterationLimit() {
			return iterationLimit;
		}

		void SetIterationLimit(int iterationLimit) {
			this->iterationLimit = iterationLimit;
		}

		class UCTNode {
		protected:
			// rewards for each agent
			AgentsReward rewards;
			int visits = 0;

		public:
			// adds rewards and increments visits
			void Update(AgentsReward rew) {
				if (rewards.GetAgentsNum() == 0) rewards = AgentsReward(rew.GetAgentsNum());
				rewards.Merge(rew);
				visits++;
			}

			int GetVisits() {
				return visits;
			}

			int GetReward(int agentIndex) {
				return rewards.GetReward(agentIndex);
			}
		};

		template<class, class> class ActionNode;
		template<class, class> class StateNode;


		template<class S, class A>
		class StateNode : public UCTNode {
		protected:
			S state;
			vector<spt<ActionNode<S, A>>> children;
			UCTAgent* agent;

		public:
			StateNode(S state, vector<A> legalActions, UCTAgent* agent) : agent(agent) {
				this->state = state;
				this->children = vector<spt<ActionNode<S, A>>>();

				for (auto& act : legalActions) {
					this->children.push_back(spt<ActionNode<S, A>>(new ActionNode<S, A>(act, agent)));
				}
			}

			// select child node with best UCT value; always play a random unexplored action first
			spt<ActionNode<S, A>> UctSelect() {
				if (visits <= children.size()) {
					vector<spt<ActionNode<S, A>>> unvisited = vector<spt<ActionNode<S, A>>>();

					for (auto child : children) {
						if (child->GetVisits() == 0) unvisited.push_back(child);
					}

					return unvisited[(int)(ofRandom(0, 1)*unvisited.size())];
				}
				else {
					spt<ActionNode<S, A>> result = spt<ActionNode<S, A>>();
					double bestUCT = 0;
					double uctValue;
					for (auto child : children) {
						uctValue = ((double)child->GetReward(state.GetAgentOnTurn())) / child->GetVisits() +
							agent->uctParameter * sqrt(log(GetVisits()) / child->GetVisits()) +
							(ofRandom(0, 1)*0.000000005 - 0.000000005 / 2);

						if (!result || uctValue > bestUCT) {
							bestUCT = uctValue;
							result = child;
						}
					}
					return result;
				}
			}

			S& GetState() {
				return state;
			}

			vector<spt<ActionNode<S, A>>>& GetChildren() {
				return children;
			}

			vector<A> GetLegalActions() {
				vector<A> legalActions = vector<A>();

				for (auto child : children) {
					legalActions.push_back(child->GetAction());
				}
				return legalActions;
			}
		};

		template<class S, class A>
		class ActionNode : public UCTNode {
		protected:
			A action;
			vector<spt<StateNode<S, A>>> frequencyTable;
			unordered_map<S, spt<StateNode<S, A>>> children;
			UCTAgent* agent;

		public:
			ActionNode(A action, UCTAgent* agent) :agent(agent) {
				this->action = action;
				frequencyTable.clear();

				if (agent->stateBranchSize != -1) {
					children = unordered_map<S, spt<StateNode<S, A>>>(agent->stateBranchSize);
				}
				else {
					children = unordered_map<S, spt<StateNode<S, A>>>();

				}
			}

			// will take an action from the current simulator's state; create a new state
			// node at the next state and return that state node. if sparse sampling limit
			// has been reach then a random node is returned from the current list of children (this is faster)
			spt<StateNode<S, A>> SelectChild(spt<Simulator<S, A>> simulator) {
				if (agent->stateBranchSize == -1 || visits < agent->stateBranchSize) {
					spt<Simulator<S, A>> clone = simulator->DeepCopy();
					clone->MakeAction(action);
					S state = clone->GetActualState();

					spt<StateNode<S, A>> stateNode = children[state];

					if (!stateNode) {
						stateNode = spt<StateNode<S, A>>(new StateNode<S, A>(state, clone->GetPossibleActions(), agent));
						children[state] = stateNode;
					}
					return stateNode;
				}
				else {
					if (frequencyTable.empty()) {
						for (auto stateNode : children) {
							for (int i = 0; i < stateNode.second->GetVisits(); i++) {
								frequencyTable.push_back(stateNode.second);
							}
						}
						children.clear();
					}

					return frequencyTable[(int)(ofRandom(0, 1)*frequencyTable.size())];
				}
			}

			A GetAction() {
				return action;
			}
		};

		/**
		* Table that stores number of all rewards and visits; needed for ensemble
		*/
		class RewardTable {
		private:
			double** rewards;
			int** visits;
			int numberOfUCTTrees = 0;
			int actions = 0;
		public:

			RewardTable(int numberOfUCTTrees, int actions) {
				Allocate(numberOfUCTTrees, actions);
			}

			~RewardTable() {
				Clear();
			}

			int GetNumberOfUCTTrees() {
				return numberOfUCTTrees;
			}

			int GetActionNum() {
				return actions;
			}

			void Allocate(int numberOfUCTTrees, int actions) {

				this->numberOfUCTTrees = numberOfUCTTrees;
				this->actions = actions;

				rewards = new double*[numberOfUCTTrees];
				visits = new int*[numberOfUCTTrees];

				for (int i = 0; i < numberOfUCTTrees; i++) {
					rewards[i] = new double[actions];
					visits[i] = new int[actions];

					memset(rewards[i], 0, sizeof(double)*actions);
					memset(visits[i], 0, sizeof(int)*actions);

				}
			}

			void Clear() {
				// deallocate arrays
				for (int i = 0; i < numberOfUCTTrees; i++) {
					delete[] rewards[i];
					delete[] visits[i];
				}

				delete[] rewards;
				delete[] visits;
			}

			inline void StoreReward(int i, int j, double val) {
				rewards[i][j] = val;
			}

			inline void StoreVisit(int i, int j, int val) {
				visits[i][j] = val;
			}

			inline void StoreData(int i, int j, double reward, int visit) {
				rewards[i][j] = reward;
				visits[i][j] = visit;
			}

			inline double GetReward(int i, int j) {
				return rewards[i][j];
			}

			inline int GetVisit(int i, int j) {
				return visits[i][j];
			}
		};


	public:

		UCTAgent() {

		}

		// for sparse sampling of large stochastic state space
		// uctConstant = controls balance between exploration and exploitation
		// sparseSampleSize = max number of sample states from any action node or infinite if equal to -1
		UCTAgent(string name, int numberOfSims, double uctParameter)
			: numberOfSims(numberOfSims), uctParameter(uctParameter)
		{
			this->name = name;
			this->baseAgent = spt<AIAgent>(new RandomAgent<S, A>());
		}

		UCTAgent(string name, int numberOfSims, double uctParameter, int iterationLimit)
			: UCTAgent(name, numberOfSims, uctParameter)
		{
			this->iterationLimit = iterationLimit;
		}

		// builds UCT trees and then selects the best action
		// if the number of trajectories is less than the number of actions at the root state
		// then not all actions are explored at least one time. In this situation the best action
		// is selected from only those that have been explored
		virtual A ChooseAction(spt<Simulator<S, A>> simulator) {

			// get actions
			auto actions = simulator->GetPossibleActions();

			if (actions.size() == 1) return actions[0];
			// get index of actual agent
			int agentOnTurn = simulator->GetActualState().GetAgentOnTurn();


			// allocate 2D array of rewards per each action per each ensemble
			RewardTable rewardTable = RewardTable(numberOfUCTTrees, actions.size());

			for (int i = 0; i < this->numberOfUCTTrees; i++) {
				// create root node
				spt<StateNode<S, A>> root = spt<StateNode<S, A>>(new StateNode<S, A>(simulator->GetActualState(), actions, this));

				// play simulations
				for (int j = 0; j < this->numberOfSims; j++) {
					PlaySimulation(root, simulator->DeepCopy());
				}

				// fill array according to the results, stored in the tree
				auto children = root->GetChildren();

				for (int j = 0; j < children.size(); j++) {
					if (children[j]->GetVisits() > 0) {
						rewardTable.StoreReward(i, j, children[j]->GetReward(agentOnTurn));
						rewardTable.StoreVisit(i, j, children[j]->GetVisits());
					}
				}
			}

			// select action index
			int actionIndex = SelectActionIndex(rewardTable);

			// return selected action
			return actions[actionIndex];
		}

	private:


		int SelectActionIndex(RewardTable& rewardTable) {

			int actionIndex = 0;
			int size = rewardTable.GetActionNum();

			if (calcMethod == UCTCalcMethod::HIGHEST_REWARD) {
				// sum all rewards and visits
				auto rewards = new double[size];
				memset(rewards, 0, sizeof(double)*size);
				auto visits = new int[size];
				memset(visits, 0, sizeof(int)*size);

				for (int i = 0; i < numberOfUCTTrees; i++) {
					for (int j = 0; j < size; j++) {
						rewards[j] += rewardTable.GetReward(i, j);
						visits[j] += rewardTable.GetVisit(i, j);
					}
				}
				// select action with the best reward/visit ratio
				for (int i = 1; i < size; i++) {
					if (visits[i] > 0 && (visits[actionIndex] == 0 || (rewards[i] / visits[i]) >(rewards[actionIndex] / visits[actionIndex]))) {
						actionIndex = i;
					}
				}

				delete[] rewards;
				delete[] visits;
			}
			else if (calcMethod == UCTCalcMethod::BEST_AVG_REWARD) {

				// at each index, the array holds, how many times had the action
				// under the selected index best avg ratio
				auto actionsVotes = new int[size];
				memset(actionsVotes, 0, sizeof(int)*size);

				for (int i = 0; i < numberOfUCTTrees; i++) {
					int bestAvgRewardIndex = -1;
					double bestAvgReward = 0;

					// for each action, calculate reward/visit ratio
					for (int j = 0; j < size; j++) {
						if (rewardTable.GetVisit(i, j) > 0) {
							double avgReward = rewardTable.GetReward(i, j) / rewardTable.GetVisit(i, j);
							if (bestAvgRewardIndex == -1 || avgReward > bestAvgReward) {
								bestAvgRewardIndex = j;
								bestAvgReward = avgReward;
							}
						}
					}
					actionsVotes[bestAvgRewardIndex] += 1;
				}

				vector<int> selectedVotes = vector<int>();
				selectedVotes.push_back(0);

				for (int i = 1; i < size; i++) {
					// store only indices with the highest vote
					if (actionsVotes[i] >= actionsVotes[selectedVotes[0]]) {
						if (actionsVotes[i] > actionsVotes[selectedVotes[0]]) {
							// delete previous votes, because a higher number was found
							selectedVotes.clear();
						}
						selectedVotes.push_back(i);
					}
				}
				// select randomly the index, because the array contains now only indices with the same vote
				actionIndex = selectedVotes[(int)(ofRandom(0, 1)*selectedVotes.size())];

				delete[] actionsVotes;
			}

			return actionIndex;
		}

		/**
		* Starts the simulation for the selected state node; walks down the tree until it reaches an
		* unexplored leaf; then it simulates a random game to explore its values
		*/
		AgentsReward PlaySimulation(spt<StateNode<S, A>> node, spt<Simulator<S, A>> simulator) {
			COGMEASURE_BEGIN("PlaySimulation");
			AgentsReward rewards;

			if (simulator->IsDeadEnd() || node->GetVisits() == 0) {
				// dead end or unexplored leaf node -> play random game
				rewards = SimulateRandomGame(simulator);
			}
			else {
				rewards = PlaySimulation(node->UctSelect(), simulator);
			}

			node->Update(rewards);
			COGMEASURE_END("PlaySimulation");
			return rewards;
		}

		/**
		* Starts the simulation for the selected action node
		*/
		AgentsReward PlaySimulation(spt<ActionNode<S, A>> node, spt<Simulator<S, A>> simulator) {
			spt<StateNode<S, A>> child = node->SelectChild(simulator);
			simulator->SetActualState(child->GetState(), child->GetLegalActions());
			auto rewards = PlaySimulation(child, simulator);
			node->Update(rewards);
			return rewards;
		}

		/**
		* Simulates random game for unexplored leaf node
		* Returns sum of all rewards; simulation can be limited with the maximum numbner of iterations
		*/
		AgentsReward SimulateRandomGame(spt<Simulator<S, A>> simulator) {
			COGMEASURE_BEGIN("SimulateRandomGame");

			AgentsReward totalRewards = simulator->GetRewards();
			COGASSERT(totalRewards.GetAgentsNum() != 0, "UCTAgent", "AgentsReward not initialized! Expected number of agents!");

			int iteration = 0;

			while (!simulator->IsDeadEnd() && (iterationLimit == -1 || iteration <= iterationLimit)) {

				// standard simulation
				A action = baseAgent->ChooseAction(simulator->DeepCopy());
				simulator->MakeAction(action);

				for (int i = 0; i < totalRewards.GetAgentsNum(); i++) {
					totalRewards.AddVal(i, simulator->GetRewards().GetReward(i));
				}

				iteration++;
			}

			COGMEASURE_END("SimulateRandomGame");
			return totalRewards;
		}


		friend class UCTNode;
	};

} // namespace