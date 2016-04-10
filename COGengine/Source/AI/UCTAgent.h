#pragma once

#include <vector>

#include "StrId.h"
#include "AIAgent.h"
#include "ofMath.h"
#include "ofxSmartPointer.h"
#include "RandomAgent.h"
#include "AgentsReward.h"
#include "Definitions.h"

namespace Cog {


	/**
	* Table that stores number of all rewards and visits; needed for ensemble
	*/
	class WinTable {
	private:
		float* wins;
		float* defeats;
		int* visits;
		int actions = 0;
	public:

		WinTable(int actions) {
			Allocate(actions);
		}

		~WinTable() {
			Clear();
		}

		int GetActionNum() {
			return actions;
		}

		// allocates table
		void Allocate(int actions) {
			this->actions = actions;

			wins = new float[actions];
			defeats = new float[actions];
			visits = new int[actions];
			memset(wins, 0, sizeof(float)*actions);
			memset(defeats, 0, sizeof(float)*actions);
			memset(visits, 0, sizeof(int)*actions);
		}

		// deallocate table
		void Clear() {
			delete[] wins;
			delete[] defeats;
			delete[] visits;
		}

		inline void AddWins(int actionIndex, float num) {
			wins[actionIndex] += num;
		}

		inline void AddDefeats(int actionIndex, float num) {
			defeats[actionIndex] += num;
		}

		inline void AddVisits(int actionIndex, int num) {
			visits[actionIndex] += num;
		}

		inline float GetWinsNum(int actionIndex) {
			return wins[actionIndex];
		}

		inline float GetDefeatsNum(int actionIndex) {
			return defeats[actionIndex];
		}

		inline int GetVisits(int i) {
			return visits[i];
		}
	};



	template<class S, class A>
	class UCTAgent : public AIAgent<S, A> {
	protected:
		// number of simulations
		int numberOfSims;
		// uct parameter; sets balance between exploration and exploitation
		double uctParameter;
		// iteration limit for separate game simulation
		int iterationLimit = -1;
		// base agent that is used for random play (usually random agent)
		spt<AIAgent<S, A>> baseAgent;
		// agent for opponent (usually random agent)
		spt<AIAgent<S, A>> opponentAgent;
		int agentIndex = 0;

	public:

		int GetAgentIndex() {
			return agentIndex;
		}

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

		int GetIterationLimit() {
			return iterationLimit;
		}

		void SetIterationLimit(int iterationLimit) {
			this->iterationLimit = iterationLimit;
		}

		spt<AIAgent<S, A>> GetBaseAgent() {
			return baseAgent;
		}

		void SetBaseAgent(spt<AIAgent<S, A>> agent) {
			this->baseAgent = agent;
		}

		spt<AIAgent<S, A>> GetOpponentAgent() {
			return opponentAgent;
		}

		void SetOpponentAgent(spt<AIAgent<S, A>> agent) {
			this->opponentAgent = agent;
		}

		/**
		* UCT search node; base class for both action and state nodes
		*/
		class UCTNode {
		protected:
			// sum of rewards for each agent
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

			float GetWins(int agentIndex) {
				return rewards.GetReward(agentIndex);
			}
		};

		class ActionNode;
		class StateNode;


		/**
		* State node that represents one state; has link to collection of action nodes
		* that represents actions this state can go from
		*/
		class StateNode : public UCTNode {
		protected:
			S state;
			vector<spt<ActionNode>> children;
			vector<A> possibleActions;
			UCTAgent* agent;

		public:
			StateNode(spt<Simulator<S, A>> simulator, UCTAgent* agent) : agent(agent) {
				this->state = simulator->GetActualState();
				this->possibleActions = simulator->GetPossibleActions();
				this->children = vector<spt<ActionNode>>(possibleActions.size());


				int counter = 0;
				// create node for each action
				for (auto& act : possibleActions) {
					this->children[counter++] = spt<ActionNode>(new ActionNode(act, simulator, agent));
				}
			}

			/**
			* Selects child with best UCT value
			*/
			spt<ActionNode> SelectActionNodeFromUCTValue() {
				if (this->visits <= children.size()) {

					// if number of visits is small, return random unvisited node
					auto unvisited = vector<spt<ActionNode>>();

					for (auto child : children) {
						if (child->GetVisits() == 0) unvisited.push_back(child);
					}

					return unvisited[(int)(ofRandom(0, 1)*unvisited.size())];
				}
				else {
					auto result = spt<ActionNode>();
					double bestUCT = 0;
					double uctValue;
					for (auto child : children) {

						float wins = child->GetWins(this->agent->agentIndex);
						auto childVisits = child->GetVisits();
						auto thisVisits = this->visits;

						// calculate uct value (UCB1 formula derived by Auer, Cesa-Bianchi and Fischer)
						uctValue = wins / childVisits + agent->uctParameter * sqrt(log(thisVisits) / childVisits);

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

			vector<spt<ActionNode>>& GetChildren() {
				return children;
			}
		};

		/**
		* Action node that represents one action; has link to collection of state nodes
		* that represents states this action can make
		*/
		class ActionNode : public UCTNode {
		protected:
			A action;
			spt<StateNode> child = spt<StateNode>();
			spt<Simulator<S, A>> simulator;
			UCTAgent* agent;

		public:
			ActionNode(A action, spt<Simulator<S, A>> simulator, UCTAgent* agent) :agent(agent), simulator(simulator) {
				this->action = action;
			}

			/**
			* Applies action this node represents to given simulator; finds or creates a child node according to
			* the state the simulator returns
			*/
			spt<StateNode> GetStateChild() {
				if (!child) {
					this->simulator = simulator->DeepCopy();
					simulator->MakeAction(action);
					child = spt<StateNode>(new StateNode(simulator, agent));
				}

				return child;
			}

			A& GetAction() {
				return action;
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
			this->baseAgent = spt<AIAgent<S,A>>(new RandomAgent<S, A>());
			this->opponentAgent = spt<AIAgent<S,A>>(new RandomAgent<S, A>());
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
			auto& actions = simulator->GetPossibleActions();

			if (actions.size() == 1) return actions[0];

			// get index of actual agent
			this->agentIndex = simulator->GetActualState().GetAgentOnTurn();

			// create root node
			spt<StateNode> root = spt<StateNode>(new StateNode(simulator, this));

			// play simulations
			for (int j = 0; j < this->numberOfSims; j++) {
				PlaySimulation(root, simulator->DeepCopy(), agentIndex);
			}

			// fill array according to the results, stored in the tree
			auto children = root->GetChildren();

			// allocate 2D array of rewards per each action per each ensemble
			WinTable winTable = WinTable(actions.size());

			for (int j = 0; j < children.size(); j++) {
				auto child = children[j];

				if (child->GetVisits() > 0) {
					winTable.AddVisits(j, child->GetVisits());
					winTable.AddWins(j, child->GetWins(agentIndex));
					winTable.AddDefeats(j, child->GetWins(agentIndex == 0 ? 1 : 0));
				}
			}


			// select action index
			int actionIndex = SelectActionIndex(winTable);

			// return selected action
			return actions[actionIndex];
		}

	private:


		int SelectActionIndex(WinTable& winTable) {

			int bestIndex = 0;
			float bestValue = -1000;

			int actions = winTable.GetActionNum();

			for (int i = 0; i<actions; i++) {
				float defeats = winTable.GetDefeatsNum(i);
				float wins = winTable.GetWinsNum(i);
				int visits = winTable.GetVisits(i);

				if (visits > 0) {
					float value = (wins - defeats * 10) * ((float)visits);
					if (value > bestValue) {
						bestValue = value;
						bestIndex = i;
					}
				}
			}

			return bestIndex;
		}

		/**
		* Starts the simulation for the selected state node; walks down the tree until it reaches an
		* unexplored leaf; then it simulates a random game to explore its values
		*/
		AgentsReward PlaySimulation(spt<StateNode> node, spt<Simulator<S, A>> simulator, int thisAgentIndex) {
			AgentsReward rewards;

			if (simulator->IsDeadEnd() || node->GetVisits() == 0) {
				// dead end or unexplored leaf node -> play random game
				rewards = SimulateRandomGame(simulator, thisAgentIndex);
			}
			else {
				auto uctNode = node->SelectActionNodeFromUCTValue();
				spt<StateNode> child = uctNode->GetStateChild();
				simulator->SetActualState(child->GetState(), child->GetPossibleActions());
				rewards = PlaySimulation(child, simulator, thisAgentIndex);
				uctNode->UpdateRewardsAndVisits(rewards);
			}

			node->UpdateRewardsAndVisits(rewards);
			return rewards;
		}


		/**
		* Simulates random game for unexplored leaf node
		* Returns sum of all rewards; simulation can be limited with the maximum numbner of iterations
		*/
		AgentsReward SimulateRandomGame(spt<Simulator<S, A>> simulator, int thisAgentIndex) {
			int iteration = 0;
			AgentsReward partialRewards = AgentsReward(2);

			while (!simulator->IsDeadEnd() && (iterationLimit == -1 || iteration < iterationLimit)) {

				A action;

				if (simulator->GetActualState().GetAgentOnTurn() == thisAgentIndex) {
					action = baseAgent->ChooseAction(simulator->DeepCopy());
				}
				else {
					action = opponentAgent->ChooseAction(simulator->DeepCopy());
				}

				simulator->MakeAction(action);
				partialRewards.Merge(simulator->GetRewards());
				iteration++;
			}

			AgentsReward totalRewards = AgentsReward(2);

			if (simulator->IsDeadEnd()) {
				// if there is only one step to victory, use higher reward
				int reward = iteration > 1 ? 1 : 100;

				if (simulator->GetActualState().GetAgentOnTurn() == thisAgentIndex) {
					totalRewards.AddVal(thisAgentIndex == 0 ? 1 : 0, reward);
				}
				else {
					totalRewards.AddVal(thisAgentIndex, reward);
				}
			}
			else {
				if (partialRewards.GetReward(0) > partialRewards.GetReward(1)) {
					totalRewards.AddVal(thisAgentIndex == 0 ? 0 : 1, 0.02f);
				}
				else {
					totalRewards.AddVal(thisAgentIndex == 0 ? 1 : 0, 0.02f);
				}
			}

			return totalRewards;
		}


		friend class UCTNode;
	};

} // namespace