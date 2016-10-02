#include <LuaBridge.h>

#include "FacadeLua.h"
#include "Definitions.h"
#include "Node.h"
#include "ComponentStorage.h"
#include "Tween.h"
#include "Stage.h"
#include "EnumConverter.h"
#include "LuaScripting.h"

namespace Cog {
	void FacadeLua::CogSwitchToScene(string name, string tweenDirection) {
		TweenDirection twDir = EnumConverter::StrToTweenDirection(tweenDirection);
		
		auto sceneContext = GETCOMPONENT(Stage);
		auto scene = sceneContext->FindSceneByName(name);
		sceneContext->SwitchToScene(scene, twDir);
	}

	void FacadeLua::CogSwitchBackToScene(string tweenDirection) {
		TweenDirection twDir = EnumConverter::StrToTweenDirection(tweenDirection);

		auto sceneContext = GETCOMPONENT(Stage);
		sceneContext->SwitchBackToScene(twDir);
	}

	int FacadeLua::CogGetFrameCounter() {
		return Cog::CogGetFrameCounter();
	}
	
	int FacadeLua::CogGetAbsoluteTime() {
		return (int)Cog::CogGetAbsoluteTime();
	}

	bool FacadeLua::CogIsKeyPressed(int key) {
		return Cog::CogIsKeyPressed(key);
	}

	Vec2i FacadeLua::CogGetMousePosition() {
		return Cog::CogGetMousePosition();
	}
	
	Vec2i FacadeLua::CogGetMouseScroll() {
		return Cog::CogGetMouseScroll();
	}
	
	int FacadeLua::CogGetScreenWidth() {
		return Cog::CogGetScreenWidth();
	}
	
	int FacadeLua::CogGetVirtualWidth() {
		return Cog::CogGetVirtualWidth();
	}
	
	int FacadeLua::CogGetScreenHeight() {
		return Cog::CogGetScreenHeight();
	}
	
	int FacadeLua::CogGetVirtualHeight() {
		return Cog::CogGetVirtualHeight();
	}
	
	float FacadeLua::CogGetVirtualAspectRatio() {
		return Cog::CogGetVirtualAspectRatio();
	}
	
	float FacadeLua::CogGetScreenAspectRatio() {
		return Cog::CogGetScreenAspectRatio();
	}
	
	Vec2i FacadeLua::CogGetScreenSize() {
		return Cog::CogGetScreenSize();
	}
	
	Vec2i FacadeLua::CogGetVirtualScreenSize() {
		return Cog::CogGetVirtualScreenSize();
	}


	void FacadeLua::CogLogError(string module, string message) {
		Cog::CogLogError(module.c_str(), message.c_str());
	}
	
	void FacadeLua::CogLogInfo(string module, string message) {
		Cog::CogLogInfo(module.c_str(), message.c_str());
	}
	
	void FacadeLua::CogLogDebug(string module, string message) {
		Cog::CogLogDebug(module.c_str(), message.c_str());
	}

	void FacadeLua::CogStopAllSounds() {
		Cog::CogStopAllSounds();
	}

	int FacadeLua::CogRegisterBehaviorPrototypeCt(luabridge::lua_State* L) {
		return GETCOMPONENT(LuaScripting)->RegisterBehaviorPrototypeCt(L);
	}

	void FacadeLua::InitMapping(luabridge::lua_State* L) {
		luabridge::getGlobalNamespace(L)
			.addFunction("CogGetAbsoluteTime", &FacadeLua::CogGetAbsoluteTime)
			.addFunction("CogGetFrameCounter", &FacadeLua::CogGetFrameCounter)
			.addFunction("CogGetMousePosition", &FacadeLua::CogGetMousePosition)
			.addFunction("CogGetMouseScroll", &FacadeLua::CogGetMouseScroll)
			.addFunction("CogGetScreenAspectRatio", &FacadeLua::CogGetScreenAspectRatio)
			.addFunction("CogGetScreenHeight", &FacadeLua::CogGetScreenHeight)
			.addFunction("CogGetScreenSize", &FacadeLua::CogGetScreenSize)
			.addFunction("CogGetScreenWidth", &FacadeLua::CogGetScreenWidth)
			.addFunction("CogGetVirtualAspectRatio", &FacadeLua::CogGetVirtualAspectRatio)
			.addFunction("CogGetVirtualHeight", &FacadeLua::CogGetVirtualHeight)
			.addFunction("CogGetVirtualScreenSize", &FacadeLua::CogGetVirtualScreenSize)
			.addFunction("CogGetVirtualWidth", &FacadeLua::CogGetVirtualWidth)
			.addFunction("CogIsKeyPressed", &FacadeLua::CogIsKeyPressed)
			.addFunction("CogLogDebug", &FacadeLua::CogLogDebug)
			.addFunction("CogLogError", &FacadeLua::CogLogError)
			.addFunction("CogLogInfo", &FacadeLua::CogLogInfo)
			.addFunction("CogSwitchBackToScene", &FacadeLua::CogSwitchBackToScene)
			.addFunction("CogSwitchToScene", &FacadeLua::CogSwitchToScene)
			.addFunction("CogStopAllSounds", &FacadeLua::CogStopAllSounds)
			.addCFunction("CogRegisterBehaviorPrototype", &FacadeLua::CogRegisterBehaviorPrototypeCt);
	}
}