#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for movement
	*/
	class Move : public Behavior {
		OBJECT_PROTOTYPE(Move)
	protected:
		bool infiniteBoard;
	public:
		// todo: refactor
		Move(bool infiniteBoard): infiniteBoard(infiniteBoard) {

		}

		void OnStart();

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace