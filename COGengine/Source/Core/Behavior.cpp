#include "Behavior.h"
#include "Node.h"
#include "Scene.h"

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

	void Behavior::SendMessage(HandlingType handlingType, StrId action, int subaction, MsgEvent* data, Node* source) const {
		Msg msg(handlingType, action, subaction, id, source, data);
		owner->GetScene()->SendMessage(msg, source);
	}

	void Behavior::SendMessageToListeners(StrId action, int subaction, MsgEvent* data, Node* source) const {
		Msg msg(HandlingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
		owner->GetScene()->SendMessage(msg, source);
	}

	void Behavior::SendDirectMessage(StrId action, int subaction, MsgEvent* data, Node* source, int targetId) const {
		Msg msg(HandlingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
		owner->GetScene()->SendDirectMessageToListener(msg, targetId);
	}



}// namespace