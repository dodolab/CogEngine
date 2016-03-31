#pragma once

#include "AIState.h"

class HydroqState : public AIState {
public:
	int aiRigs;
	int playerRigs;
	int emptyRigs;
	
	HydroqState() {

	}

	HydroqState(const HydroqState& copy) {
		aiRigs = copy.aiRigs;
		playerRigs = copy.playerRigs;
		emptyRigs = copy.emptyRigs;
		agentOnTurn = copy.agentOnTurn;
	}

	HydroqState(int aiRigs, int playerRigs, int emptyRigs, int agentOnTurn) : aiRigs(aiRigs), playerRigs(playerRigs), emptyRigs(emptyRigs)
	{
		this->agentOnTurn = agentOnTurn;
	}

	void SwapAgentOnTurn() {
		this->agentOnTurn = (agentOnTurn == 0) ? 1 : 0;
	}

	int GetHashCode() const {
		return aiRigs * 1000000 + playerRigs * 10000 + emptyRigs * 100 + GetAgentOnTurn();
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
		return rhs.agentOnTurn == agentOnTurn && rhs.aiRigs == aiRigs && rhs.emptyRigs == emptyRigs && rhs.playerRigs == playerRigs;
	}


};

namespace std {
	// overriding hash function for position
	template <>
	struct hash<HydroqState> {
		inline size_t operator()(const HydroqState& state) const {
			return state.GetHashCode();
		}
	};
}
