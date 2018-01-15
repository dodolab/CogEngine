#pragma once

#include <string>
using namespace std;

#include "Definitions.h"
#include "MsgLua.h"
#include "Settings.h"

namespace luabridge {
	struct lua_State;
}

namespace Cog {

	/**
	* Component wrapper for Lua script
	*/
	class ComponentLua {
	public:
		static void InitLuaMapping(luabridge::lua_State* L);
	};

} // namespace