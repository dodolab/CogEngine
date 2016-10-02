#include "MsgLua.h"
#include "Msg.h"
#include "NodeLua.h"
#include <LuaBridge.h>

namespace Cog {

	StrId MsgLua::GetAction() const {
		return msg->GetAction();
	}

	bool MsgLua::HasAction(StrId action) const {
		return msg->HasAction(action);
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

	void MsgLua::InitMapping(luabridge::lua_State* L) {
		// Msg proxy
		luabridge::getGlobalNamespace(L)
			.beginClass<MsgLua>("Msg")
			.addProperty("action", &MsgLua::GetAction)
			.addProperty("senderId", &MsgLua::GetSenderId)
			.addProperty("contextNode", &MsgLua::GetContextNode)
			.addProperty("parameter", &MsgLua::GetParameter)
			.addFunction("HasAction", &MsgLua::HasAction)
			.endClass();
	}

} // namespace