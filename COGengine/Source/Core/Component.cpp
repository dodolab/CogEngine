#include "Component.h"

namespace Cog {

	void Component::SendMessage(HandlingType handlingType, StrId action, int subaction, MsgEvent* data, Node* source) const {
		Msg msg(handlingType, action, subaction, id, source, data);
		CogSendMessage(msg, source);
	}

	void Component::SendMessageToListeners(StrId action, int subaction, MsgEvent* data, Node* source) const {
		CogSendMessageToListeners(action, subaction, data, source, id);
	}

	void Component::SendDirectMessage(StrId action, int subaction, MsgEvent* data, Node* source, int targetId) const {
		CogSendDirectMessageToListener(action, subaction, data, source, targetId, id);
	}

} // namespace