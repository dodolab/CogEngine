#include "BehaviorLua.h"
#include "MsgLua.h"
#include "Node.h"
#include "ofxCogEngine.h"
#include "LuaScripting.h"
#include "ComponentStorage.h"
#include "NodeLua.h"
#include "LuaBridge.h"
#include "SceneLua.h"

using namespace luabridge;

namespace Cog {

	BehaviorLua::BehaviorLua() {
		// objects allocated in lua always have to be 
		//external (they aren't deallocated in Node's destructor)
		this->isExternal = true; 

		auto scr = GETCOMPONENT(LuaScripting);
		L = scr->GetLua();
		scr->StoreBehavior(this);
	}

	void BehaviorLua::OnInit() {
		if (this->ownerLua == nullptr) {
			this->ownerLua = new NodeLua(owner);
			SetOwnerLua(ownerLua);
		}

		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto init = ref["OnInit"];
		COGASSERT(!init.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		init(ref);
	}

	int BehaviorLua::RegisterDelegateCt(luabridge::lua_State* L) {
		COGASSERT(lua_gettop(L) == 3, "BehaviorLua", "Wrong registration call! Expected two parameters: registered object and its name");
		
		// get behavior name and register this object
		string name = lua_tostring(L, 3);
		REGISTER_LUABEHAVIOR(name, this);
		
		// remove string parameter and save only the object
		lua_pop(L, 1);

		int r = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_rawgeti(L, LUA_REGISTRYINDEX, r);
		this->reference = r;
		return 0; // number of return values
	}

	void BehaviorLua::SendMessage(StrId msg) {
		Behavior::SendMessage(msg);
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

	void BehaviorLua::OnMessage(Msg& msg) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto method = ref["OnMessage"];
		COGASSERT(!method.isNil(), "BehaviorLua", "Wrong lua object; expected method OnMessage");
		method(ref, MsgLua(&msg));
	}

	void BehaviorLua::Update(const uint64 delta, const uint64 absolute) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		
		COGASSERT(!ref.isNil(), "BehaviorLua", "Wrong lua object; expected reference");
		auto method = ref["Update"];
		COGASSERT(!method.isNil(), "BehaviorLua", "Wrong lua object; expected method Update");
		method(ref, (int)delta, (int)absolute);
	}

	void BehaviorLua::SubscribeForMessagesLua(StrId action) {
		SubscribeForMessages(action);
	}

	SceneLua BehaviorLua::GetScene() {
		return SceneLua(this->owner->GetScene());
	}

} // namespace