#pragma once

#include "Behavior.h"

namespace Cog {

	/**
	* Behavior for movement, updates transformations according
	* to the Movement attribute
	*/
	class Move : public Behavior {
	protected:
		// indicator, if the moving space is infinite
		bool infiniteBoard;
	public:
		Move() {

		}

		/**
		* Creates a new moving behavior
		* @param infiniteBoard indicator whether the moving space is infinite
		*/
		Move(bool infiniteBoard): infiniteBoard(infiniteBoard) {

		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace