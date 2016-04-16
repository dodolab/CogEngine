#pragma once

#include "Behavior.h"
#include "Msg.h"

namespace Cog {

	/**x
	* Behavior that sends message with delay to specific behavior
	*/
	class DelayAction : public Behavior {
	private:
		// number of milliseconds for delay
		int millis = 0;
		// actual time
		int actual = 0;
		// id of target behavior
		int targetId = -1;
		// message payload
		spt<MsgEvent> data;

	public:

		DelayAction() {

		}

		/**
		* Creates a new behavior that sends message to another behavior with delay
		* @param millis number of milliseconds for delay
		* @param targetId id of target whom the message will be sent to
		* @param data message payload
		*/
		DelayAction(int millis, int targetId, spt<MsgEvent> data) : millis(millis), targetId(targetId), data(data) {

		}

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace