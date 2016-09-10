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

		int hashCode = 0;

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
			this->hashCode = copy.hashCode;
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

		int GetHashCode() const {
			return hashCode + this->agentOnTurn;
		}

		int GetBlueRigsNum() {
			return distancesBlue.size();
		}

		int GetRedRigsNum() {
			return distancesRed.size();
		}

		void Recalc() {
			hashCode = 0;

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

			for (auto dist : distancesRed) {
				hashCode = HashCombine(hashCode, dist);
			}

			for (auto dist : distancesBlue) {
				hashCode = HashCombine(hashCode, dist);
			}

			for (auto dist : distancesRedEmpty) {
				hashCode = HashCombine(hashCode, dist);
			}
		}

		inline bool operator==(const HydroqState& a) const {
			return GetHashCode() == a.GetHashCode();
		}

		inline bool operator!=(const HydroqState& a) const {
			return GetHashCode() == a.GetHashCode();
		}

		inline bool operator<(const HydroqState& a) const {
			return GetHashCode() < a.GetHashCode();
		}

		inline bool operator>(const HydroqState& a) const {
			return GetHashCode() > a.GetHashCode();
		}

	public:

		bool operator !=(const HydroqState& rhs) { return !(*this == rhs); }

		bool operator ==(const HydroqState& rhs) {
			return GetHashCode() != rhs.GetHashCode();
		}
	};


} // namespace

namespace std {
	// overriding hash function for position
	template <>
	struct hash<Cog::HydroqState> {
		inline size_t operator()(const Cog::HydroqState& state) const {
			return state.GetHashCode();
		}
	};
}