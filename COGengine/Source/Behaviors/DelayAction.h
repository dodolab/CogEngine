#pragma once

#include "Behavior.h"
#include "Msg.h"

namespace Cog {

	/**
	* Behavior that sends message with delay 
	*/
	class DelayAction : public Behavior {
	private:
		// message to send
		Msg message;
		// absolute time the message should be delivered
		uint64 deliverTime;
	public:

		DelayAction() {

		}

		/**
		* Creates a new behavior that sends message with delay
		* @param message message to send
		* @param deliverTime the absolute time the message should be delivered
		*/
		DelayAction(Msg message, uint64 deliverTime) : message(message), deliverTime(deliverTime) {

		}

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace