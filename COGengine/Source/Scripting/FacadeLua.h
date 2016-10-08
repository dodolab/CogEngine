#pragma once

#include <string>
using namespace std;

#include "StrId.h"
#include "Vec2i.h"

namespace luabridge {
	struct lua_State;
}

namespace Cog {

	class ComponentLua;
	class ResourcesLua;
	class StageLua;

	/**
	* Facade for Lua scripts
	*/
	class FacadeLua {
	public:
		// singletons
		static ResourcesLua* resLua;
		static StageLua* stageLua;

		// =================== Components ======================
		static ComponentLua* CogGetComponent(string name);

		static void CogSwitchToScene(string name, string tweenDirection);
		static void CogSwitchBackToScene(string tweenDirection);
		// =================== ofxCogEngine ====================

		static int CogGetFrameCounter();
		static int CogGetAbsoluteTime();

		// =================== Environment ====================

		static bool CogIsKeyPressed(int key);
		static Vec2i CogGetMousePosition();
		static Vec2i CogGetMouseScroll();
		static int CogGetScreenWidth();
		static int CogGetVirtualWidth();
		static int CogGetScreenHeight();
		static int CogGetVirtualHeight();
		static float CogGetVirtualAspectRatio();
		static float CogGetScreenAspectRatio();
		static Vec2i CogGetScreenSize();
		static Vec2i CogGetVirtualScreenSize();
		

		// =================== Logger =========================

		static void CogLogError(string module, string message);
		static void CogLogInfo(string module, string message);
		static void CogLogDebug(string module, string message);
		
		static void CogStopAllSounds();

		// =================== Raw functions =========================

		/**
		* Registers behavior prototype that is called from Extend() function
		*/
		static int CogRegisterBehaviorPrototypeCt(luabridge::lua_State* L);

		static void InitLuaMapping(luabridge::lua_State* L);
	};

} // namespace