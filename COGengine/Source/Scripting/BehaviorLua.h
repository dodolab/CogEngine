#pragma once

#include "Behavior.h"
#include "MsgLua.h"
#include "SceneLua.h"

namespace luabridge {
	struct lua_State;
}

namespace Cog {

	/**
	* Behavior wrapper for Lua scripts
	*/
	class BehaviorLua : public Behavior {
	private:
		int reference = 0;
		luabridge::lua_State* L;
	public:
		
		NodeLua* ownerLua = nullptr;

		BehaviorLua();

		~BehaviorLua() {

		}

		void OnInit();

		int RegisterDelegateCt(luabridge::lua_State* L);

		void SendMessage(StrId msg);

		void SetOwnerLua(NodeLua* ownerLua);

		virtual void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

		void SubscribeForMessagesLua(StrId action);

		SceneLua GetScene();
	};

} // namespace