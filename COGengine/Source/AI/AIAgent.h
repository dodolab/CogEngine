#pragma once

#include <string>
#include "StrId.h"
#include "Simulator.h"
#include "ofxSmartPointer.h"

using namespace std;

namespace Cog {

	/**
	* Game agent that choses action at each step of the game
	* Behavior of the agent is specified by derived classes (e.g. random agent, human agent, UCT agent,...)
	* @tparam S state the agent operates with
	* @tparam A action the agent and the state operate with
	*/
	template<class S, class A>
	class AIAgent {
	protected:
		// name of the agent
		string name;
	public:
		AIAgent()  {

		}

		AIAgent(string name) : name(name) {

		}

		/**
		* Chooses action from collection of possible actions, provided by given simulator
		* The simulator can be used for internal simulation, because it is deep copied (see UCTAgent)
		*/
		virtual A ChooseAction(spt<Simulator<S, A>> simulator) = 0;

		/**
		* Gets agent's name
		*/
		string GetName() const {
			this->name = name;
		}

		/**
		* Sets agent's name
		*/
		void SetName(string name) {
			this->name = name;
		}
	};


} // namespace