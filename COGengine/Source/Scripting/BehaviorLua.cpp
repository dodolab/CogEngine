#include "BehaviorLua.h"
#include "MsgLua.h"
#include "Node.h"
#include "CogEngine.h"
#include "LuaScripting.h"

using namespace luabridge;

namespace Cog {

	BehaviorLua::BehaviorLua() {
		auto scr = GETCOMPONENT(LuaScripting);
		L = scr->GetLua();
	//	reference = luaL_ref(L, LUA_REGISTRYINDEX);
		int mojo = 12;
	}

	void BehaviorLua::OnMessage(Msg msg) {
		//OnMessageLua(MsgLua(&msg));
	}

	void BehaviorLua::Update(const uint64 delta, const uint64 absolute) {
		auto scr = GETCOMPONENT(LuaScripting);
		 L = scr->GetLua();
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref(L);
		auto update = ref["Update"];
		update(delta, absolute);
	}

	void BehaviorLua::RegisterListeningLua(string action) {
		RegisterListening(owner->GetScene(), StringHash(action));
	}

} // namespace