#include "BehaviorLua.h"
#include "MsgLua.h"
#include "Node.h"
#include "CogEngine.h"
#include "LuaScripting.h"

using namespace luabridge;

namespace Cog {

	BehaviorLua::BehaviorLua() {
		// objects allocated in lua always have to be external (aren't deallocated in Node's destructor)
		this->isExternal = true; 
		auto scr = GETCOMPONENT(LuaScripting);
		L = scr->GetLua();
		scr->StoreBehavior(this);
	}

	void BehaviorLua::OnInit() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto init = ref["OnInit"];
		COGASSERT(!init.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		init();
	}

	int BehaviorLua::RegisterBehaviorCt(luabridge::lua_State* L) {
		int r = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_rawgeti(L, LUA_REGISTRYINDEX, r);
		this->reference = r;
		return 0; // number of return values
	}

	void BehaviorLua::SendMessage(StringHash msg) {
		this->SendMessageToListeners(msg, 0, nullptr, this->owner);
	}

	void BehaviorLua::SetOwnerLua(NodeLua* ownerLua) {
		this->ownerLua = ownerLua;
		this->owner = ownerLua->node;
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto owner = ref["owner"];
		owner.rawset(ownerLua);
	}

	void BehaviorLua::OnMessage(Msg msg) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto method = ref["OnMessage"];
		COGASSERT(!method.isNil(), "BehaviorLua", "Wrong lua object; expected method OnMessage");
		method(MsgLua(&msg));
	}

	void BehaviorLua::Update(const uint64 delta, const uint64 absolute) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto method = ref["Update"];
		COGASSERT(!method.isNil(), "BehaviorLua", "Wrong lua object; expected method OnUpdate");
		method((int)delta, (int)absolute);
	}

	void BehaviorLua::RegisterListeningLua(string action) {
		RegisterListening(StringHash(action));
	}

} // namespace