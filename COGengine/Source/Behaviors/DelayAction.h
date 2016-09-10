#pragma once

#include "Behavior.h"
#include "TranslateAnim.h"
#include "Msg.h"

namespace Cog {

	/**x
	* Behavior that sends message with delay to specific behavior
	*/
	class DelayAction : public Behavior {
		OBJECT_PROTOTYPE(DelayAction)
	private:
		// number of milliseconds for delay
		int millis = 0;
		// actual time
		int actual = 0;
		// id of target behavior
		int targetId = -1;
		// message payload
		MsgEvent* data = nullptr;

	public:

		/**
		* Creates a new behavior that sends message to another behavior with delay
		* @param millis number of milliseconds for delay
		* @param targetId id of target whom the message will be sent to
		* @param data message payload
		*/
		DelayAction(int millis, int targetId, MsgEvent* data) : millis(millis), targetId(targetId), data(data) {

		}

		virtual void Update(const uint64 delta, const uint64 absolute) {
			actual += (int)delta;

			if (actual > millis) {
				SendDirectMessage(ACT_DELAY_ACTION, 0, data, owner, targetId);
				Finish();
			}
		}


	};

}// namespace