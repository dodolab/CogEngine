#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for movement
	*/
	class Move : public Behavior {
	protected:
		bool infiniteBoard;
	public:
		Move() {

		}

		// todo: refactor
		Move(bool infiniteBoard): infiniteBoard(infiniteBoard) {

		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace