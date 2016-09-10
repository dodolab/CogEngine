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
		// number of UCT trees for evaluation
		int numberOfUCTTrees = 1;
		// iteration limit for separate game simulation
		int iterationLimit = -1;
		// base agent that is used for random play (usually random agent)
		spt<AIAgent<S, A>> baseAgent;
		// calculation method, used for final action selection
		UCTCalcMethod calcMethod = UCTCalcMethod::HIGHEST_REWARD;

	public:

		int GetNumberOfSimulations() {
			return numberOfSims;
		}

		void SetNumberOfSimulations(int numberOfSims) {
			this->numberOfSims = numberOfSims;
		}

		double GetUCTParameter() {
			return uctParameter;
		}

		void SetUCTParameter(double uctParameter) {
			this->uctParameter = uctParameter;
		}

		int GetNumberOfUCTTrees() {
			return numberOfUCTTrees;
		}

		void SetNumberOfUCTTrees(int numberOfUCTTrees) {
			this->numberOfUCTTrees = numberOfUCTTrees;
		}

		int GetIterationLimit() {
			return iterationLimit;
		}

		void SetIterationLimit(int iterationLimit) {
			this->iterationLimit = iterationLimit;
		}

		UCTCalcMethod GetCalcMethod() {
			return calcMethod;
		}

		void SetCalcMethod(UCTCalcMethod calcMethod) {
			this->calcMethod = calcMethod;
		}

		/**
		* UCT search node; base class for both action and state nodes
		*/
		class UCTNode {
		protected:
			// rewards for each agent
			AgentsReward rewards;
			// number of visits
			int visits = 0;

		public:
			// adds rewards and increments visits
			void UpdateRewardsAndVisits(AgentsReward rew) {
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


		/**
		* State node that represents one state; has link to collection of action nodes 
		* that represents actions this state can go from
		*/
		template<class S, class A>
		class StateNode : public UCTNode {
		protected:
			S state;
			vector<spt<ActionNode<S, A>>> children;
			vector<A> possibleActions;
			UCTAgent* agent;

		public:
			StateNode(S state, vector<A> possibleActions, UCTAgent* agent) : agent(agent) {
				this->state = state;
				this->children = vector<spt<ActionNode<S, A>>>();
				this->possibleActions = possibleActions;

				// create node for each action
				for (auto& act : possibleActions) {
					this->children.push_back(spt<ActionNode<S, A>>(new ActionNode<S, A>(act, agent)));
				}
			}

			/**
			* Selects child with best UCT value
			*/
			spt<ActionNode<S, A>> SelectActionNodeFromUCTValue() {
				if (visits <= children.size()) {
					
					// if number of visits is small, return random unvisited node
					auto unvisited = vector<spt<ActionNode<S, A>>>();

					for (auto child : children) {
						if (child->GetVisits() == 0) unvisited.push_back(child);
					}

					return unvisited[(int)(ofRandom(0, 1)*unvisited.size())];
				}
				else {
					auto result = spt<ActionNode<S, A>>();
					double bestUCT = 0;
					double uctValue;
					for (auto child : children) {

						auto reward = (double)child->GetReward(state.GetAgentOnTurn());
						
						// calculate uct value (UCB1 formula derived by Auer, Cesa-Bianchi and Fischer)
						uctValue = reward / child->GetVisits() + agent->uctParameter * sqrt(log(GetVisits()) / child->GetVisits());

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

			vector<A>& GetPossibleActions() {
				return possibleActions;
			}

			vector<spt<ActionNode<S, A>>>& GetChildren() {
				return children;
			}
		};

		/**
		* Action node that represents one action; has link to collection of state nodes
		* that represents states this action can make
		*/
		template<class S, class A>
		class ActionNode : public UCTNode {
		protected:
			A action;
			vector<spt<StateNode<S, A>>> children;
			UCTAgent* agent;

		public:
			ActionNode(A action, UCTAgent* agent) :agent(agent) {
				this->action = action;
				children.clear();
			}

			/**
			* Applies action this node represents to given simulator; finds or creates a child node according to
			* the state the simulator returns
			*/
			spt<StateNode<S, A>> SelectStateNodeFromAction(spt<Simulator<S, A>> simulator) {
				
				// simulate action and obtain state
				spt<Simulator<S, A>> clone = simulator->DeepCopy();
				clone->MakeAction(action);
				S& state = clone->GetActualState();

				// search for state node in the children collection
				spt<StateNode<S, A>> stateNode;

				for (auto child : children) {
					if (child->GetState() == state) {
						stateNode = child;
						break;
					}
				}

				if (!stateNode) {
					stateNode = spt<StateNode<S, A>>(new StateNode<S, A>(state, clone->GetPossibleActions(), agent));
					children.push_back(stateNode);
				}
				return stateNode;
			}

			A& GetAction() {
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

			// allocates table
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

			// deallocate table
			void Clear() {
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

		/**
		* Creates a new UCT agent
		* @param name name of the agent
		* @param numberOfSims number of simulations for one action selection
		* @param uctParameter parameter of exploration parameter (theoretically equal to sqrt(2))
		*/
		UCTAgent(string name, int numberOfSims, double uctParameter)
			: numberOfSims(numberOfSims), uctParameter(uctParameter)
		{
			this->name = name;
			this->baseAgent = spt<AIAgent>(new RandomAgent<S, A>());
		}


		/**
		* Creates a new UCT agent
		* @param name name of the agent
		* @param numberOfSims number of simulations for one action selection
		* @param uctParameter parameter of exploration parameter (theoretically equal to sqrt(2))
		* @param iterationLimit maximum number of iterations for one simulation
		*/
		UCTAgent(string name, int numberOfSims, double uctParameter, int iterationLimit)
			: UCTAgent(name, numberOfSims, uctParameter)
		{
			this->iterationLimit = iterationLimit;
		}


		/**
		* Builds UCT trees and choses the best action, according to the selected configuration
		* The best action is selected from only those that have been explored
		*/
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
				// play simulation from the selected node
				rewards = PlaySimulation(node->SelectActionNodeFromUCTValue(), simulator);
			}
			
			node->UpdateRewardsAndVisits(rewards);
			COGMEASURE_END("PlaySimulation");
			return rewards;
		}

		/**
		* Starts the simulation from the selected action node
		*/
		AgentsReward PlaySimulation(spt<ActionNode<S, A>> node, spt<Simulator<S, A>> simulator) {
			// use the action node to find the stateNode by applying the appropriate action to the simulator
			spt<StateNode<S, A>> child = node->SelectStateNodeFromAction(simulator);
			// now set the new state to the simulator
			simulator->SetActualState(child->GetState(), child->GetPossibleActions());
			
			// play simulation with updated simulator and get rewards
			auto rewards = PlaySimulation(child, simulator);
			// update rewards
			node->UpdateRewardsAndVisits(rewards);
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