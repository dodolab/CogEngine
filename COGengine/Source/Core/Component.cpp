#include "Component.h"
#include "CogEngine.h"
#include "Stage.h"
#include "Scene.h"

namespace Cog {


	void Component::SendMessage(StrId action, spt<MsgEvent> data, Node* contextNode) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, contextNode, data);
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessage(StrId action, spt<MsgEvent> data) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, nullptr, data);
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessage(StrId action, Node* contextNode) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, contextNode, spt<MsgEvent>());
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

	void Component::SendMessage(StrId action) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::SUBSCRIBERS, nullptr, spt<MsgEvent>());
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}


	void Component::SendMessageToBehavior(StrId action, spt<MsgEvent> data, Node* contextNode, int recipientId) const {
		Msg msg(action, MsgObjectType::COMPONENT, this->id, MsgObjectType::BEHAVIOR, recipientId, TunnelingMode::TUNNELING, contextNode, data);
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}

} // namespace