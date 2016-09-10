#pragma once

#include <string>
#include "StrId.h"
#include "Simulator.h"
#include "ofxSmartPointer.h"

using namespace std;

namespace Cog {

	/**
	* Game agent that choses action at each step of the game
	* It could have various types of behavior (e.g. random agent, human agent or UCT agent)
	*/
	template<class S, class A>
	class AIAgent {
	protected:
		// identifier incremental counter
		static int idCounter;
		const int id;
		string name;
	public:
		AIAgent() : id(idCounter++) {

		}

		AIAgent(string name) : AIAgent() {
			this->name = name;
		}

		/**
		* Chooses action from collection of possible actions, provided by given simulator
		* Clone of this simulator can be used for further simulations for agent's purposes (e.g. UCT agent)
		*/
		virtual A ChooseAction(spt<Simulator<S, A>> simulator) = 0;

		int GetId() {
			return id;
		}

		string GetName() {
			this->name = name;
		}

		void SetName(string name) {
			this->name = name;
		}
	};

	template<class S, class A>
	int AIAgent<S, A>::idCounter = 0;

} // namespace