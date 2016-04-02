#pragma once

#include "AIState.h"
#include "Vec2i.h"
#include "Utils.h"

namespace Cog {

	class HydroqState : public AIState {
	public:
		vector<int> distancesRed;
		vector<int> distancesBlue;
		vector<int> distancesBlueEmpty;
		vector<int> distancesRedEmpty;

		HydroqState() {

		}

		HydroqState(int agentOnTurn) {
			this->agentOnTurn = agentOnTurn;
		}

		HydroqState(const HydroqState& copy) {
			this->distancesRed = copy.distancesRed;
			this->distancesBlue = copy.distancesBlue;
			this->distancesBlueEmpty = copy.distancesBlueEmpty;
			this->distancesRedEmpty = copy.distancesRedEmpty;
			this->agentOnTurn = copy.agentOnTurn;
		}

		void RemoveRedRig(int index) {
			int counter = 0;
			for (auto it = distancesRed.begin(); it != distancesRed.end(); ++it) {
				if (counter++ == index) {
					distancesRed.erase(it);
					break;
				}
			}
		}

		void RemoveBlueRig(int index) {
			int counter = 0;
			for (auto it = distancesBlue.begin(); it != distancesBlue.end(); ++it) {
				if (counter++ == index) {
					distancesBlue.erase(it);
					break;
				}
			}
		}

		void RemoveEmptyRig(int index) {
			int counter = 0;
			for (auto it = distancesBlueEmpty.begin(); it != distancesBlueEmpty.end(); ++it) {
				if (counter++ == index) {
					distancesBlueEmpty.erase(it);
					break;
				}
			}

			counter = 0;
			for (auto it = distancesRedEmpty.begin(); it != distancesRedEmpty.end(); ++it) {
				if (counter++ == index) {
					distancesRedEmpty.erase(it);
					break;
				}
			}
		}

		void PrintSomething() {
			cout << "MODRA-RUDA: ";
			for (auto dist : distancesRed) {
				cout << dist << ", " ;
			}
			cout << endl;

			cout << "RUDA-MODRA: ";
			for (auto dist : distancesBlue) {
				cout << dist << ", ";
			}
			cout << endl;
			cout << "MODRA_PRAZDNA: ";
			for (auto dist : distancesBlueEmpty) {
				cout << dist << ", ";
			}
			cout << endl;
			cout << "RUDA_PRAZDNA: ";
			for (auto dist : distancesRedEmpty) {
				cout << dist << ", ";
			}
			cout << endl;
		}

		void SwapAgentOnTurn() {
			this->agentOnTurn = (agentOnTurn == 0) ? 1 : 0;
		}

		int GetBlueRigsNum() {
			return distancesBlue.size();
		}

		int GetRedRigsNum() {
			return distancesRed.size();
		}

		void Recalc() {
			sort(distancesRed.begin(), distancesRed.end(),
				[](const int & a, const int & b) -> bool
			{
				return a > b;
			});
			sort(distancesBlue.begin(), distancesBlue.end(),
				[](const int & a, const int & b) -> bool
			{
				return a > b;
			});
			sort(distancesRedEmpty.begin(), distancesRedEmpty.end(),
				[](const int & a, const int & b) -> bool
			{
				return a > b;
			});

			sort(distancesBlueEmpty.begin(), distancesBlueEmpty.end(),
				[](const int & a, const int & b) -> bool
			{
				return a > b;
			});
		}

		bool operator ==(const HydroqState& rhs) const 
		{ 
			for (int i = 0; i < distancesRed.size(); i++) {
				if (distancesRed[i] != rhs.distancesRed[i]) return false;
			}

			for (int i = 0; i < distancesBlue.size(); i++) {
				if (distancesBlue[i] != rhs.distancesBlue[i]) return false;
			}

			for (int i = 0; i < distancesRedEmpty.size(); i++) {
				if (distancesRedEmpty[i] != rhs.distancesRedEmpty[i]) return false;
			}

			for (int i = 0; i < distancesBlueEmpty.size(); i++) {
				if (distancesBlueEmpty[i] != rhs.distancesBlueEmpty[i]) return false;
			}
		}

		bool operator !=(const HydroqState& rhs) const { return !(*this == rhs); }


	};


} // namespace
