#include "ComponentStorage.h"
#include "LuaScripting.h"
#include "BehaviorLua.h"

namespace Cog {

	Behavior* ComponentStorage::CreateBehaviorPrototype(StrId key) {
		auto it = behaviorBuilders.find(key);

		if (it == behaviorBuilders.end()) {
			auto luaScripting = GETCOMPONENT(LuaScripting);
			// if the behavior couldn't be found in C++ objects, try to find it in Lua script
			auto luaBeh = luaScripting->CreateLuaBehavior(key);
			if (luaBeh == nullptr) COGLOGDEBUG("ComponentStorage", "Behavior prototype %s doesn't exists", key.GetStringValue().c_str());
			else return luaBeh;
		}

		auto builder = it->second;
		return builder->Create();
	}

} // namespace