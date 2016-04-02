#pragma once

#include "Simulator.h"
#include "HydroqState.h"
#include "HydroqAction.h"
#include "Error.h"
#include "StringHash.h"

namespace Cog {

#define AGENT_AI 0
#define AGENT_PLAYER 1


	class HydroqSimulator : public Simulator<HydroqState, HydroqAction>
	{
	public:
		int totalRigs = 0;

		HydroqSimulator() {
			this->agentsNumber = 2;
		}

		void InitState() {
			this->actualState = HydroqState(AGENT_PLAYER);
			this->actualState.distancesBlue.push_back(5);
			this->actualState.distancesRed.push_back(5);
			this->actualState.distancesBlueEmpty.push_back(2);
			this->actualState.distancesRedEmpty.push_back(2);
			this->actualState.Recalc();

			this->totalRigs = 4;

			RecalcPossibleActions();
			this->rewards = AgentsReward(0, 0);
		}

		spt<Simulator> DeepCopy() {
			auto copy = spt<HydroqSimulator>(new HydroqSimulator());
			copy->actualState = this->actualState;
			copy->agentsNumber = this->agentsNumber;
			copy->possibleActions = this->possibleActions;
			copy->rewards = this->rewards;
			return copy;
		}

		void MakeAction(HydroqAction act, bool isSimulation) {
			if (find(this->possibleActions.begin(), this->possibleActions.end(), act) == this->possibleActions.end()) {
				throw IllegalOperationException("Wrong action to take!");
			}

			int index = act.index;

			if (act.type == HydroqActionType::CAPTURE_ENEMY_RIG) {
				if (actualState.GetAgentOnTurn() == AGENT_AI) {
					if(!isSimulation) cout << "Rudej lajznul modrou" << endl;
					actualState.RemoveBlueRig(index);
					actualState.distancesRed.push_back(1);
					rewards = AgentsReward(10, -10);
				}
				else {
					if (!isSimulation) cout << "Modrej lajznul rudou" << endl;
					actualState.RemoveRedRig(index);
					actualState.distancesBlue.push_back(1);
					rewards = AgentsReward(-10, 10);
				}
			}
			else if (act.type == HydroqActionType::CAPTURE_EMPTY_RIG) {
				if (actualState.GetAgentOnTurn() == AGENT_AI) {
					if (!isSimulation) cout << "Rudej lajznul prazdnou" << endl;
					actualState.distancesRed.push_back(actualState.distancesRedEmpty[index]);
					actualState.RemoveEmptyRig(index);
					rewards = AgentsReward(10, 0);
				}
				else {
					if (!isSimulation) cout << "Modrej lajznul prazdnou" << endl;
					actualState.distancesBlue.push_back(actualState.distancesBlueEmpty[index]);
					actualState.RemoveEmptyRig(index);
					rewards = AgentsReward(0, 10);
				}
			}
			else if (act.type == HydroqActionType::GO_TO_EMPTY_RIG) {
				if (actualState.GetAgentOnTurn() == AGENT_AI) {
					if (!isSimulation) cout << "Rudej jde k prazdne" << endl;
					actualState.distancesRedEmpty[index]--;
					rewards = AgentsReward(2, 0);
				}
				else {
					if (!isSimulation) cout << "Modrej jde k prazdne" << endl;
					actualState.distancesBlueEmpty[index]--;
					rewards = AgentsReward(0, 2);
				}
			}
			else if (act.type == HydroqActionType::GO_TO_ENEMY_RIG) {
				if (actualState.GetAgentOnTurn() == AGENT_AI) {
					if (!isSimulation) cout << "Rudej jde k modre" << endl;
					actualState.distancesBlue[index]--;
					rewards = AgentsReward(1, 0);
				}
				else {
					if (!isSimulation) cout << "Modrej jde k rude" << endl;
					actualState.distancesRed[index]--;
					rewards = AgentsReward(0, 1);
				}
			}

			if (!isSimulation) actualState.PrintSomething();

			this->actualState.SwapAgentOnTurn();
			this->actualState.Recalc();
			RecalcPossibleActions();
			if (!isSimulation) PrintPossibleStates();
		}

	protected:

		void PrintPossibleStates() {
			for (auto m : possibleActions) {
				if (m.type == HydroqActionType::CAPTURE_EMPTY_RIG) cout << "CAPTURE_EMPTY_RIG" << ", ";
				else if (m.type == HydroqActionType::CAPTURE_ENEMY_RIG) cout << "CAPTURE_ENEMY_RIG" << ", ";
				else if (m.type == HydroqActionType::GO_TO_EMPTY_RIG) cout << "GO_TO_EMPTY_RIG" << ", ";
				else if (m.type == HydroqActionType::GO_TO_ENEMY_RIG) cout << "GO_TO_ENEMY_RIG" << ", ";
			}
			cout << endl;
		}

		virtual void RecalcPossibleActions() {
			this->possibleActions.clear();

			if (actualState.distancesBlue.empty() || actualState.distancesRed.empty()) return;

			// 1) find all agent's rigs
			auto& distancesToEnemy = actualState.GetAgentOnTurn() == AGENT_AI ? actualState.distancesBlue : actualState.distancesRed;
			auto& distancesToEmpty = actualState.GetAgentOnTurn() == AGENT_AI ? actualState.distancesRedEmpty : actualState.distancesBlueEmpty;

			for (int i = 0; i < distancesToEnemy.size(); i++) {
				if (distancesToEnemy[i] == 0) {
					possibleActions.push_back(HydroqAction(HydroqActionType::CAPTURE_ENEMY_RIG, i));
				}
				else {
					possibleActions.push_back(HydroqAction(HydroqActionType::GO_TO_ENEMY_RIG, i));
				}
			}

			for (int i = 0; i < distancesToEmpty.size(); i++) {
				if (distancesToEmpty[i] == 0) {
					possibleActions.push_back(HydroqAction(HydroqActionType::CAPTURE_EMPTY_RIG, i));
				}
				else{
					possibleActions.push_back(HydroqAction(HydroqActionType::GO_TO_EMPTY_RIG, i));
				}
			}

		}

	};

} // namespace