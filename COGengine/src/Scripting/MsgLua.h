#pragma once

#include "StrId.h"

namespace luabridge {
	struct lua_State;
}


namespace Cog {

	class Msg;
	class NodeLua;

	/**
	* Msg wrapper for Lua script
	*/
	class MsgLua {
		Msg* msg;
	public:

		MsgLua(Msg* msg):msg(msg) {
		}

		StrId GetAction() const;
		bool HasAction(StrId action) const;
		int GetParameter() const;
		int GetSenderId() const;
		NodeLua* GetContextNode() const;

		static void InitLuaMapping(luabridge::lua_State* L);
	};

} // namespace