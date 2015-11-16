#pragma once

#include "Behavior.h"
#include "TranslateAnim.h"
#include "Msg.h"

/**x
* Behavior that sends message with delay to specific behavior
*/
class DelayAction : public Behavior{
private:
	// number of milliseconds for delay
	int millis;
	// actual time
	int actual;
	// id of target behavior
	int targetId;
	// message payload
	void* data;

public:

	/**
	* Creates a new behavior that sends message to another behavior with delay
	* @param millis number of milliseconds for delay
	* @param targetId id of target whom the message will be sent to
	* @param data message payload
	*/
	DelayAction(int millis, int targetId, void* data) :  millis(millis), actual(0), targetId(targetId), data(data){

	}

	virtual void Update(const uint64 delta, const uint64 absolute){
		actual += delta;

		if (actual > millis){
			SendDirectMessage(Actions::DELAY_ACTION,0, data, owner, targetId);
			Finish();
		}
	}


};
