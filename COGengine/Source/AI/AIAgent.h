#pragma once

#include <string>
#include "StringHash.h"
#include "Simulator.h"
#include "ofxSmartPointer.h"

using namespace std;

namespace Cog {

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