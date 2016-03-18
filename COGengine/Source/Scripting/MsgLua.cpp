#include "MsgLua.h"
#include "Msg.h"
#include "NodeLua.h"

namespace Cog {

	StringHash MsgLua::GetAction() const {
		return msg->GetAction();
	}

	int MsgLua::GetSubAction() const {
		return msg->GetSubaction();
	}

	int MsgLua::GetBehaviorId() const {
		return msg->GetBehaviorId();
	}

	NodeLua* MsgLua::GetSourceObj() const {
		return new NodeLua(msg->GetSourceObject());
	}

} // namespace