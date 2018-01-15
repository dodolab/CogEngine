#include "ComponentStorage.h"
#ifdef LUA_SCRIPTING
#include "LuaScripting.h"
#endif
#include "BehaviorLua.h"


namespace Cog {

	Behavior* ComponentStorage::CreateBehaviorPrototype(StrId key) {
		auto it = behaviorBuilders.find(key);

#ifdef LUA_SCRIPTING
		if (it == behaviorBuilders.end()) {
			auto luaScripting = GETCOMPONENT(LuaScripting);
			// if the behavior couldn't be found in C++ objects, try to find it in Lua script
			auto luaBeh = luaScripting->CreateLuaBehavior(key);
			if (luaBeh == nullptr) CogLogError("ComponentStorage", "Behavior prototype %s doesn't exists", key.GetStringValue().c_str());
			else return luaBeh;
		}
#endif

		if (it == behaviorBuilders.end()) {
			CogLogError("ComponentStorage", "Behavior prototype %s doesn't exists", key.GetStringValue().c_str());
			return nullptr;
		}
		else {
			auto builder = it->second;
			return builder->Create();
		}
	}

} // namespace