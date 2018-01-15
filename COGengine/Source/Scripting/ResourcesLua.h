#pragma once

#include <string>
using namespace std;

#include "Definitions.h"
#include "MsgLua.h"
#include "Settings.h"
#include "ComponentLua.h"
#include "ResourcesMgr.h"

namespace luabridge {
	struct lua_State;
}


namespace Cog {

	class GeneralAnim;
	class SpriteSheet;

	/**
	* Resources wrapper for Lua script
	*/
	class ResourcesLua : public ComponentLua {
	public:
		Resources* res = nullptr;

		ResourcesLua() {

		}

		~ResourcesLua() {
		}

		ofImage* Get2DImage(string path);

		Setting GetDefaultSettings(string name);

		Setting GetGlobalSettings(string name);

		Setting GetProjectSettings(string name);

		string GetResourceStr(string resourceType, string resourceKey);

		Resources* GetRes();

		static void InitLuaMapping(luabridge::lua_State* L);
	};

} // namespace