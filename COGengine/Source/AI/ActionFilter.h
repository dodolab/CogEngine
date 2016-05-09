#pragma once

#include "Definitions.h"

namespace Cog {

	/**
	* Action filter that may be used to limit state space of a simulated game
	*/
	template<class S, class A>
	class ActionFilter {
	public:
		virtual void ApplyFilter(S& actualState, vector<A>& possibleActions) = 0;
	};

} // namespace
