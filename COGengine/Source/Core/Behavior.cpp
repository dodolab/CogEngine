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

	void Behavior::SubscribeForMessages(StrId action1) {
		MsgListener::SubscribeForMessages(owner->GetScene(), action1);
	}


	void Behavior::SubscribeForMessages(StrId action1, StrId action2) {
		MsgListener::SubscribeForMessages(owner->GetScene(), action1);
		MsgListener::SubscribeForMessages(owner->GetScene(), action2);
	}

	void Behavior::SubscribeForMessages(StrId action1, StrId action2, StrId action3) {
		MsgListener::SubscribeForMessages(owner->GetScene(), action1);
		MsgListener::SubscribeForMessages(owner->GetScene(), action2);
		MsgListener::SubscribeForMessages(owner->GetScene(), action3);
	}

	void Behavior::SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4) {
		MsgListener::SubscribeForMessages(owner->GetScene(), action1);
		MsgListener::SubscribeForMessages(owner->GetScene(), action2);
		MsgListener::SubscribeForMessages(owner->GetScene(), action3);
		MsgListener::SubscribeForMessages(owner->GetScene(), action4);
	}

	void Behavior::SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4, StrId action5) {
		MsgListener::SubscribeForMessages(owner->GetScene(), action1);
		MsgListener::SubscribeForMessages(owner->GetScene(), action2);
		MsgListener::SubscribeForMessages(owner->GetScene(), action3);
		MsgListener::SubscribeForMessages(owner->GetScene(), action4);
		MsgListener::SubscribeForMessages(owner->GetScene(), action5);
	}

}// namespace