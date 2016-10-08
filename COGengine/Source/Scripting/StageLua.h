#pragma once

#include <string>
using namespace std;

#include "Definitions.h"
#include "MsgLua.h"
#include "Settings.h"
#include "ComponentLua.h"

namespace luabridge {
	struct lua_State;
}


namespace Cog {

	class Stage;
	class SceneLua;

	/**
	* Stage wrapper for Lua script
	*/
	class StageLua : public ComponentLua {
	public:
		Stage* stage = nullptr;

		StageLua();

		NodeLua* GetRootObject() const;

		SceneLua* GetActualScene() const;

		void AddScene(SceneLua* scene, bool setAsActual);

		SceneLua* FindSceneByName(string name) const;

		void SwitchToScene(SceneLua* scene, string tweenDir);

		bool SwitchBackToScene(string tweenDir);

		static void InitLuaMapping(luabridge::lua_State* L);
	};

} // namespace