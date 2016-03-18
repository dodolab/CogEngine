#pragma once

#include "Behavior.h"
#include "MsgLua.h"

namespace luabridge {
	class lua_State;
}

namespace Cog {


	class BehaviorLua : public Behavior {
	private:
		int reference = 0;
		luabridge::lua_State* L;
	public:
		
		
		BehaviorLua();


		void Init() {
			bool mojo = true;
		}

		int UpdateLua(luabridge::lua_State* L) {
			bool mojo = false;
			return 0;
		}

		void OnMessage(Msg msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

		void RegisterListeningLua(string action);
	};

} // namespace