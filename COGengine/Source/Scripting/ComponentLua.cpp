#include "ComponentLua.h"
#include "Facade.h"
#include "ComponentStorage.h"
#include <LuaBridge.h>

namespace Cog {

	void ComponentLua::InitLuaMapping(luabridge::lua_State* L) {
		luabridge::getGlobalNamespace(L)
			.beginClass<ComponentLua>("Component")
			.endClass();
	}

} // namespace