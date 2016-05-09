#pragma once

#include <vector>

#include "StrId.h"
#include "AIAgent.h"
#include "ofMath.h"
#include "RandomAgent.h"
#include "AgentsReward.h"
#include "Definitions.h"

namespace Cog {


	/**
	* Table that stores number of wins, defeats and visits for each action
	*/
	class WinTable {
	private:
		// array of wins
		float* wins;
		// array of defeats
		float* defeats;
		// array of visits
		int* visits;
		// number of actions
		int actions = 0;
	public:

		WinTable(int actions) {
			Allocate(actions);
		}

		~WinTable() {
			Clear();
		}

		/**
		* Gets number of actions
		*/
		int GetActionNum() const{
			return actions;
		}

		/**
		* Allocates the table with given number of actions
		*/
		void Allocate(int actions) {
			this->actions = actions;

			wins = new float[actions];
			defeats = new float[actions];
			visits = new int[actions];
			// set zero for all items
			memset(wins, 0, sizeof(float)*actions);
			memset(defeats, 0, sizeof(float)*actions);
			memset(visits, 0, sizeof(int)*actions);
		}

		/**
		* Deallocates the table
		*/
		void Clear() {
			delete[] wins;
			delete[] defeats;
			delete[] visits;
		}

		/**
		* Adds number of wins for action at selected index
		* @param actionIndex index of selected action
		* @param num number of wins
		*/
		inline void AddWins(int actionIndex, float num) {
			wins[actionIndex] += num;
		}

		/**
		* Adds number of defeats for action at selected index
		* @param actionIndex index of selected action
		* @param num number of defeats
		*/
		inline void AddDefeats(int actionIndex, float num) {
			defeats[actionIndex] += num;
		}

		/**
		* Adds number of visits for action at selected index
		* @param actionIndex index of selected action
		* @param num number of visits
		*/
		inline void AddVisits(int actionIndex, int num) {
			visits[actionIndex] += num;
		}

		/**
		* Gets number of wins for action at selected index
		*/
		inline float GetWinsNum(int actionIndex) const{
			return wins[actionIndex];
		}

		/**
		* Gets number of defeats for action at selected index
		*/
		inline float GetDefeatsNum(int actionIndex) const {
			return defeats[actionIndex];
		}

		/**
		* Gets number of visits for action at selected index
		*/
		inline int GetVisits(int i) const{
			return visits[i];
		}
	};

	/**
	* Agent for Monte Carlo Tree Search (UCT is for Uppper Confidence Bound applied to trees)
	* For the action selection it creates a tree based on exploration/exploitation parameter and
	* simulates random games to collect statistical data such as number of wins, defeats and visits of corresponding node
	* 
	* The tree has two types of node : action nodes that actually represents edges of the Monte Carlo tree and 
	* state nodes, representing one state. Degree of state nodes is driven by the number of possible actions that may 
	* go from corresponding state
	*
	* To provide balance between exploration and exploitation, UCB1 formula is used (it has logarithmic regret)
	* For more informations, see this article: http://www.b0n541.net/study/jan_schaefer_uct_algorithm_applied_to_games_with_imperfect_information.pdf
	*/
	template<class S, class A>
	class UCTAgent : public AIAgent<S, A> {
	protected:
		// number of simulations
		int numberOfSims;
		// uct parameter; sets balance between exploration and exploitation
		double uctParameter;
		// iteration limit for interrupting game simulation
		int iterationLimit = -1;
		// base agent that is used for random play (usually random agent)
		spt<AIAgent<S, A>> baseAgent;
		// index of this agent, calculated before the simulation
		int agentIndex;
		// filter that may be used to limit collection of possible actions
		spt<ActionFilter<S, A>> actionFilter;
	public:

		/**
		* Gets number of simulations
		*/
		int GetNumberOfSimulations() const {
			return numberOfSims;
		}

		/**
		* Sets number of simulations
		*/
		void SetNumberOfSimulations(int numberOfSims) {
			this->numberOfSims = numberOfSims;
		}

		/**
		* Gets the UCT parameter that provides balance between
		* exploration and exploitation
		*/
		double GetUCTParameter() const {
			return uctParameter;
		}

		/**
		* Sets the UCT parameter
		*/
		void SetUCTParameter(double uctParameter) {
			this->uctParameter = uctParameter;
		}

		/**
		* Gets the iteration limit
		* -1 is for unlimited simulation
		*/
		int GetIterationLimit() const {
			return iterationLimit;
		}

		/**
		* Sets the iteration limit
		* -1 is for unlimited simulation
		*/
		void SetIterationLimit(int iterationLimit) {
			this->iterationLimit = iterationLimit;
		}

		/**
		* Gets the base agent
		*/
		spt<AIAgent<S, A>> GetBaseAgent() const {
			return baseAgent;
		}

		/**
		* Sets the base agent
		*/
		void SetBaseAgent(spt<AIAgent<S, A>> agent) {
			this->baseAgent = agent;
		}

		/**
		* Gets the index of current agent
		*/
		int GetAgentIndex() const {
			return agentIndex;
		}

		/**
		* Gets action filter
		*/
		spt<ActionFilter<S, A>> GetActionFilter() {
			return actionFilter;
		}

		/**
		* Sets action filter
		*/
		void SetActionFilter(spt<ActionFilter<S, A>> filter) {
			this->actionFilter = filter;
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
			/**
			* Merges rewards and increments number of visits
			*/
			void UpdateRewardsAndVisits(AgentsReward& rew) {
				if (rewards.GetAgentsNum() == 0) rewards = AgentsReward(rew.GetAgentsNum());
				rewards.Merge(rew);
				visits++;
			}

			/**
			* Gets number of visits for this node
			*/
			int GetVisits() const {
				return visits;
			}

			/**
			* Gets number of wins for agent at selected index
			*/
			float GetWins(int agentIndex) const {
				return rewards.GetReward(agentIndex);
			}
		};

		class ActionNode;
		class StateNode;

		/**
		* State node that represents one state; has link to collection of action nodes
		* that represents actions from which this state can make a transition
		*/
		class StateNode : public UCTNode {
		protected:
			S state;
			vector<spt<ActionNode>> children;
			// collection of possible actions that go from this state
			vector<A> possibleActions;
			// link to parent class
			UCTAgent* agent;

		public:
			StateNode(spt<Simulator<S, A>> simulator, UCTAgent* agent) : agent(agent) {
				this->state = simulator->GetActualState();
				this->possibleActions = simulator->GetPossibleActions();
				this->children = vector<spt<ActionNode>>(possibleActions.size());

				int counter = 0;
				// create node for each possible action
				for (auto& act : possibleActions) {
					this->children[counter++] = spt<ActionNode>(new ActionNode(act, simulator, agent));
				}
			}

			/**
			* Selects child with the best UCT value
			*/
			spt<ActionNode> SelectActionNodeFromUCTValue() {

				if (this->visits <= children.size()) {
					// if number of visits is small, return random unvisited node
					auto unvisited = vector<spt<ActionNode>>();

					// collect unvisited nodes
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
						float wins = child->GetWins(this->agent->GetAgentIndex());
						auto childVisits = child->GetVisits();
						auto thisVisits = this->visits;

						// calculate uct value (UCB1 formula created by Auer, Cesa-Bianchi and Fischer)
						uctValue = wins / childVisits + agent->uctParameter * sqrt(log(thisVisits) / childVisits);

						if (!result || uctValue > bestUCT) {
							bestUCT = uctValue;
							result = child;
						}
					}
					return result;
				}
			}

			/**
			* Gets the state this node represents
			*/
			S& GetState() {
				return state;
			}

			/**
			* Gets list of possible actions 
			*/
			vector<A>& GetPossibleActions() {
				return possibleActions;
			}

			/**
			* Gets list of children
			*/
			vector<spt<ActionNode>>& GetChildren() {
				return children;
			}
		};

		/**
		* Action node that represents an action; has link to state node with a state achievable by 
		* making a transition from parent's state node and this action
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
			* Gets state node achievable by making a transition from parent's state
			*/
			spt<StateNode> GetStateChild() {
				if (!child) {
					this->simulator = simulator->DeepCopy();
					simulator->MakeAction(action);
					child = spt<StateNode>(new StateNode(simulator, agent));
				}

				return child;
			}

			/**
			* Gets the action this node represents
			*/
			A& GetAction() const {
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
		* @param uctParameter parameter of exploration/exploitation (should be equal to sqrt(2))
		*/
		UCTAgent(string name, int numberOfSims, double uctParameter)
			: numberOfSims(numberOfSims), uctParameter(uctParameter)
		{
			this->name = name;
			this->baseAgent = spt<AIAgent<S,A>>(new RandomAgent<S, A>());
			this->actionFilter = spt<ActionFilter<S, A>>();
		}


		/**
		* Creates a new UCT agent
		* @param name name of the agent
		* @param numberOfSims number of simulations for one action selection
		* @param uctParameter parameter of exploration/exploitation (should be equal to sqrt(2))
		* @param iterationLimit maximum number of iterations for one simulation
		*/
		UCTAgent(string name, int numberOfSims, double uctParameter, int iterationLimit)
			: UCTAgent(name, numberOfSims, uctParameter)
		{
			this->iterationLimit = iterationLimit;
		}

		/**
		* Builds UCT tree and choses the best action according to the wins/defeat ration
		*/
		virtual A ChooseAction(spt<Simulator<S, A>> simulator) {

			if (actionFilter) {
				simulator->SetActionFilter(actionFilter);
				actionFilter->ApplyFilter(simulator->GetActualState(), simulator->GetPossibleActions());
			}
			
			// get possible actions
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

			// collect table of visits, wins and defeats
			WinTable winTable = WinTable(actions.size());
			FillWinTable(winTable, root, agentIndex);

			// select action index based on the winning table
			int actionIndex = SelectActionIndex(winTable);

			// return selected action
			return actions[actionIndex];
		}

	private:

		/**
		* Selects index of the best action according to the win/defeat ratio
		*/
		int SelectActionIndex(WinTable& winTable) {

			int bestIndex = 0;
			float bestValue = -1000;

			int actions = winTable.GetActionNum();

			for (int i = 0; i<actions; i++) {
				float defeats = winTable.GetDefeatsNum(i);
				float wins = winTable.GetWinsNum(i);
				int visits = winTable.GetVisits(i);

				if (visits > 0) {
					// try to avoid the index with a higher number of defeats
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
		* Starts the simulation for selected state node; walks the tree to the leaf and then it plays
		* a random game to discover rewards
		*/
		AgentsReward PlaySimulation(spt<StateNode> node, spt<Simulator<S, A>> simulator, int thisAgentIndex) {
			AgentsReward rewards;

			if (simulator->IsDeadEnd() || node->GetVisits() == 0) {
				// dead end or unexplored leaf node -> play random game
				rewards = SimulateRandomGame(simulator, thisAgentIndex);
			}
			else {
				// inner node of the tree -> walk down recursively

				// select node by UCT value, get its state and go to the lower level
				auto uctNode = node->SelectActionNodeFromUCTValue();
				spt<StateNode> child = uctNode->GetStateChild();
				simulator->SetActualState(child->GetState(), child->GetPossibleActions());
				rewards = PlaySimulation(child, simulator, thisAgentIndex);

				// back propagation - update number of visits and rewards for inner action node
				uctNode->UpdateRewardsAndVisits(rewards);
			}

			// update number of visits and rewards for inner state node
			node->UpdateRewardsAndVisits(rewards);
			return rewards;
		}


		/**
		* Simulates random game for unexplored leaf node
		* Returns sum of all rewards; simulation can be limited with the maximum number of iterations
		*/
		AgentsReward SimulateRandomGame(spt<Simulator<S, A>> simulator, int thisAgentIndex) {

			// apply action filter
			if (actionFilter) simulator->SetActionFilter(actionFilter);

			int iteration = 0;
			AgentsReward partialRewards = AgentsReward(2);
			int previousAgentIndex = 0;

			while (!simulator->IsDeadEnd() && (iterationLimit == -1 || iteration < iterationLimit)) {
				// use random agent for action selection
				A action = baseAgent->ChooseAction(simulator->DeepCopy());
				
				previousAgentIndex = simulator->GetActualState().GetAgentOnTurn();
				// make transition using the simulator
				simulator->MakeAction(action);
				// merge partial rewards
				partialRewards.Merge(simulator->GetRewards());
				iteration++;
			}

			// collect WIN-DEFEAT flags
			AgentsReward totalRewards = AgentsReward(simulator->GetAgentsNumber());
			
			if (simulator->IsDeadEnd()) {
				// if there is only one step to victory, use higher reward
				int reward = iteration > 1 ? 1 : 100;
				totalRewards.AddVal(previousAgentIndex, reward);
			}
			else {
				// simulator was interrupted before the end -> calculate partial reward as a strategic advantage
				int indexWithHighestVal = partialRewards.GetAgentIndexWithHighestReward();
				if(indexWithHighestVal != -1 ) totalRewards.AddVal(indexWithHighestVal, 0.02f);
			}



			return totalRewards;
		}

		/**
		* Fills the table with number of visits, wins and defeat for each action node of the root node
		*/
		void FillWinTable(WinTable& winTable, spt<StateNode> root, int agentIndex) {
			// fill array according to the results, stored in the tree
			auto children = root->GetChildren();

			for (auto j = 0; j < children.size(); j++) {
				auto child = children[j];

				if (child->GetVisits() > 0) {
					winTable.AddVisits(j, child->GetVisits());
					winTable.AddWins(j, child->GetWins(agentIndex));
					winTable.AddDefeats(j, child->GetWins(agentIndex == 0 ? 1 : 0));
				}
			}
		}


		friend class UCTNode;
	};

} // namespace