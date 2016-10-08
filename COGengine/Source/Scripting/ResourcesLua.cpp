#include "Resources.h"
#include "ResourcesLua.h"
#include "Facade.h"
#include "ComponentStorage.h"
#include "ComponentLua.h"
#include <LuaBridge.h>

namespace Cog {


	ofImage* ResourcesLua::Get2DImage(string path) {
		return GetRes()->Get2DImage(path).get();
	}

	Setting ResourcesLua::GetDefaultSettings(string name) {
		return GetRes()->GetDefaultSettings(name);
	}

	Setting ResourcesLua::GetGlobalSettings(string name) {
		return GetRes()->GetGlobalSettings(name);
	}

	Setting ResourcesLua::GetProjectSettings(string name) {
		return GetRes()->GetProjectSettings(name);
	}

	string ResourcesLua::GetResourceStr(string resourceType, string resourceKey) {
		return GetRes()->GetResourceStr(resourceType, resourceKey);
	}

	void ResourcesLua::InitLuaMapping(luabridge::lua_State* L) {
		luabridge::getGlobalNamespace(L)
			.deriveClass<ResourcesLua, ComponentLua>("Resources")
			.addFunction("Get2DImage", &ResourcesLua::Get2DImage)
			.addFunction("GetDefaultSettings", &ResourcesLua::GetDefaultSettings)
			.addFunction("GetGlobalSettings", &ResourcesLua::GetGlobalSettings)
			.addFunction("GetProjectSettings", &ResourcesLua::GetProjectSettings)
			.addFunction("GetResourceStr", &ResourcesLua::GetResourceStr)
			.endClass();
	}

	Resources* ResourcesLua::GetRes() {
		if (res == nullptr) {
			this->res = GETCOMPONENT(Resources);
		}

		return res;
	}

} // namespace