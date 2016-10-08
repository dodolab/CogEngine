#include "Resources.h"
#include "ResourcesLua.h"
#include "Facade.h"
#include "ComponentStorage.h"
#include "ComponentLua.h"
#include "StageLua.h"
#include "Stage.h"
#include "SceneLua.h"
#include "NodeLua.h"
#include "EnumConverter.h"
#include <LuaBridge.h>

namespace Cog {

	StageLua::StageLua() {
		stage = GETCOMPONENT(Stage);
	}

	NodeLua* StageLua::GetRootObject() const {
		return new NodeLua(stage->GetRootObject());
	}

	SceneLua* StageLua::GetActualScene() const {
		return new SceneLua(stage->GetActualScene());
	}

	SceneLua* StageLua::FindSceneByName(string name) const {
		return new SceneLua(stage->FindSceneByName(name));
	}

	void StageLua::SwitchToScene(SceneLua* scene, string tweenDir) {
		stage->SwitchToScene(scene->scene, EnumConverter::StrToTweenDirection(tweenDir));
	}

	bool StageLua::SwitchBackToScene(string tweenDir) {
		return stage->SwitchBackToScene(EnumConverter::StrToTweenDirection(tweenDir));
	}

	void StageLua::InitLuaMapping(luabridge::lua_State* L) {
		luabridge::getGlobalNamespace(L)
			.deriveClass<StageLua, ComponentLua>("Stage")
			.addProperty("rootObject", &StageLua::GetRootObject)
			.addProperty("actualScene", &StageLua::GetActualScene)
			.addFunction("FindSceneByName", &StageLua::FindSceneByName)
			.addFunction("SwitchToScene", &StageLua::SwitchToScene)
			.addFunction("SwitchBackToScene", &StageLua::SwitchBackToScene)
			.endClass();
	}

} // namespace