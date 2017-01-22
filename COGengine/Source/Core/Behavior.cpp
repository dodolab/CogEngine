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
			this->SetComponentState(ComponentState::DISABLED);
			SendMessage(ACT_BEHAVIOR_FINISHED);
			if (removeWhenFinish) owner->RemoveBehavior(this, true);
		}
	}

	void Behavior::SubscribeForMessages(StrId action1) {
		BaseComponent::SubscribeForMessages(owner->GetScene(), action1);
	}


	void Behavior::SubscribeForMessages(StrId action1, StrId action2) {
		BaseComponent::SubscribeForMessages(owner->GetScene(), action1);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action2);
	}

	void Behavior::SubscribeForMessages(StrId action1, StrId action2, StrId action3) {
		BaseComponent::SubscribeForMessages(owner->GetScene(), action1);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action2);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action3);
	}

	void Behavior::SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4) {
		BaseComponent::SubscribeForMessages(owner->GetScene(), action1);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action2);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action3);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action4);
	}

	void Behavior::SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4, StrId action5) {
		BaseComponent::SubscribeForMessages(owner->GetScene(), action1);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action2);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action3);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action4);
		BaseComponent::SubscribeForMessages(owner->GetScene(), action5);
	}

	void Behavior::SendMessage(StrId action) const {
		SendMessage(action, owner);
	}

	void Behavior::SendMessage(StrId action, Node* contextNode) const {
		Msg msg(action, MsgObjectType::BEHAVIOR, this->id, MsgObjectType::SUBSCRIBERS, contextNode, spt<MsgPayload>());
		owner->GetScene()->SendMessage(msg);
	}

	void Behavior::SendMessage(StrId action, spt<MsgPayload> data) const {
		SendMessage(action, data, owner);
	}

	void Behavior::SendMessage(StrId action, spt<MsgPayload> data, Node* contextNode) const {
		Msg msg(action, MsgObjectType::BEHAVIOR, this->id, MsgObjectType::SUBSCRIBERS, contextNode, data);
		owner->GetScene()->SendMessage(msg);
	}

	void Behavior::SendMessageToBehavior(StrId action, int recipientId) const {
		SendMessageToBehavior(action, owner, recipientId);
	}

	void Behavior::SendMessageToBehavior(StrId action, Node* contextNode, int recipientId) const {
		Msg msg(action, MsgObjectType::BEHAVIOR, this->id, MsgObjectType::BEHAVIOR, recipientId, TunnelingMode::TUNNELING, contextNode, spt<MsgPayload>());
		owner->GetScene()->SendMessage(msg);
	}

	void Behavior::SendMessageToBehavior(StrId action, spt<MsgPayload> data, int recipientId) const {
		SendMessageToBehavior(action, data, owner, recipientId);
	}

	void Behavior::SendMessageToBehavior(StrId action, spt<MsgPayload> data, Node* contextNode, int recipientId) const {
		Msg msg(action, MsgObjectType::BEHAVIOR, this->id, MsgObjectType::BEHAVIOR, recipientId, TunnelingMode::TUNNELING, contextNode, data);
		owner->GetScene()->SendMessage(msg);
	}

}// namespace