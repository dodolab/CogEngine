#pragma once

#include "Behavior.h"
#include "MsgLua.h"

namespace luabridge {
	struct lua_State;
}

namespace Cog {


	class BehaviorLua : public Behavior {
	private:
		int reference = 0;
		luabridge::lua_State* L;
	public:
		
		NodeLua* ownerLua;

		BehaviorLua();

		~BehaviorLua() {

		}

		void OnInit();

		int RegisterCt(luabridge::lua_State* L);

		void SendMessage(StringHash msg);

		void SetOwnerLua(NodeLua* ownerLua);

		void OnMessage(Msg msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

		void RegisterListeningLua(string action);
	};

} // namespace