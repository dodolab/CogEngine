#include "MsgLua.h"
#include "Msg.h"
#include "NodeLua.h"

namespace Cog {

	StrId MsgLua::GetAction() const {
		return msg->GetAction();
	}

	int MsgLua::GetParameter() const {
		return msg->GetParameter();
	}

	int MsgLua::GetSenderId() const {
		return msg->GetSenderId();
	}

	NodeLua* MsgLua::GetContextNode() const {
		return new NodeLua(msg->GetContextNode());
	}

} // namespace