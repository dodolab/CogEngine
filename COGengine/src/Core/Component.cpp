#include "Component.h"
#include "ofxCogEngine.h"
#include "Stage.h"
#include "Scene.h"

namespace Cog {

	void Component::SendMessage(StrId action) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, nullptr, new MsgPayload());
		ofxCogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessage(StrId action, Node* contextNode) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, contextNode, new MsgPayload());
		ofxCogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessage(StrId action, RefCountedObjectPtr<MsgPayload> data) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, nullptr, data);
		ofxCogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessage(StrId action, RefCountedObjectPtr<MsgPayload> data, Node* contextNode) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, contextNode, data);
		ofxCogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessageToBehavior(StrId action, RefCountedObjectPtr<MsgPayload> data, Node* contextNode, int recipientId) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::BEHAVIOR, recipientId, TunnelingMode::TUNNELING, contextNode, data);
		ofxCogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

} // namespace