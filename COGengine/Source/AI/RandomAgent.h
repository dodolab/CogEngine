#pragma once

#include "AIAgent.h"
#include "ofMath.h"

namespace Cog {

	template<class S, class A>
	class RandomAgent : public AIAgent<S,A> {
	protected:
	public:

		RandomAgent() {

		}

		RandomAgent(string name) : AIAgent(name) {

		}

		virtual A ChooseAction(spt<Simulator<S,A>> simulator) {
			auto& actions = simulator->GetPossibleActions();
			int randIndex = (int)ofRandom(0, actions.size());
			return actions[randIndex];

		}

	};


	template<class S, class A>
	class RandomAgentSmazat : public AIAgent<S, A> {
	protected:
	public:

		RandomAgentSmazat() {

		}

		RandomAgentSmazat(string name) : AIAgent(name) {

		}

		virtual A ChooseAction(spt<Simulator<S, A>> simulator) {
			cout << "Random" << endl;
			auto& actions = simulator->GetPossibleActions();
			int randIndex = (int)ofRandom(0, actions.size());
			return actions[randIndex];

		}

	};

} // namespace