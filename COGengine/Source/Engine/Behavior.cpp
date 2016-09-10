#include "Behavior.h"
#include "Node.h"

namespace Cog {

	Behavior::Behavior() {

	}

	void Behavior::Finish() {
		if (!finished) {
			finished = true;
			OnFinish();
			SendMessageToListeners(ACT_BEHAVIOR_FINISHED, this->GetId(), nullptr, owner);
			if (removeWhenFinish) owner->RemoveBehavior(this, true);
		}
	}

	void Behavior::RegisterListening(StringHash action1) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
	}


	void Behavior::RegisterListening(StringHash action1, StringHash action2) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
		MsgListener::RegisterListening(owner->GetScene(), action3);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
		MsgListener::RegisterListening(owner->GetScene(), action3);
		MsgListener::RegisterListening(owner->GetScene(), action4);
	}

	void Behavior::RegisterListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4, StringHash action5) {
		MsgListener::RegisterListening(owner->GetScene(), action1);
		MsgListener::RegisterListening(owner->GetScene(), action2);
		MsgListener::RegisterListening(owner->GetScene(), action3);
		MsgListener::RegisterListening(owner->GetScene(), action4);
		MsgListener::RegisterListening(owner->GetScene(), action5);
	}

}// namespace