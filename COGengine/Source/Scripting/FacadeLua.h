#pragma once

#include <string>
using namespace std;

#include "StrId.h"
#include "Vec2i.h"

namespace luabridge {
	struct lua_State;
}

namespace Cog {

	/**
	* Facade for Lua scripts
	*/
	class FacadeLua {
	public:
		
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
	};

} // namespace