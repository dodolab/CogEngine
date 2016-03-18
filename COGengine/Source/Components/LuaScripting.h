#pragma once

#include "ofxCogCommon.h"
#include "ofUtils.h"
#include "Component.h"

#include <LuaBridge.h>
#include <iostream>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using namespace luabridge;

#include "MsgLua.h"
#include "BehaviorLua.h"
#include "NodeLua.h"

namespace Cog {

	class LuaScripting : public Component {
		OBJECT(LuaScripting)

	private:
		lua_State* L;
	public:

		LuaScripting() {

		}

		lua_State* GetLua() {
			return L;
		}

		static int RegisterBehavior(lua_State* state) {
			GETCOMPONENT(LuaScripting)->RegisterBehaviorCt(state);
		}

		void RegisterBehaviorCt(lua_State* state) {
			int r = luaL_ref(L, LUA_REGISTRYINDEX); 
			lua_rawgeti(L, LUA_REGISTRYINDEX, r);
		}

		void Init() {
			L = luaL_newstate();
			luaL_openlibs(L);

			// ofVec2f
			getGlobalNamespace(L)
				.beginClass<ofVec2f>("ofVec2f")
				.addConstructor<void(*)(float, float)>()
				.addData("x", &ofVec2f::x)
				.addData("y", &ofVec2f::y)
				.endClass();

			// ofVec3f
			getGlobalNamespace(L)
				.beginClass<ofVec3f>("ofVec3f")
				.addConstructor<void(*)(float, float, float)>()
				.addData("x", &ofVec3f::x)
				.addData("y", &ofVec3f::y)
				.addData("z", &ofVec3f::z)
				.endClass();

			// Trans
			getGlobalNamespace(L)
				.beginClass<Trans>("Trans")
				.addConstructor <void(*) (void)>()
				.LUA_REGDATA(Trans, absPos)
				.LUA_REGDATA(Trans, absRotation)
				.LUA_REGDATA(Trans, absRotationCentroid)
				.LUA_REGDATA(Trans, absScale)
				.LUA_REGDATA(Trans, localPos)
				.LUA_REGDATA(Trans, rotation)
				.LUA_REGDATA(Trans, rotationCentroid)
				.LUA_REGDATA(Trans, scale)
				.LUA_REGDATA(Trans, absPos)
				.LUA_REGFUNC(Trans, SetAbsAsLocal)
				.LUA_REGFUNC(Trans, CalcAbsTransform)
				.LUA_REGFUNC(Trans, CalcRotationToPosition)
				.LUA_REGFUNC(Trans, SetRotationToPosition)
				.endClass();

			// StringHash
			getGlobalNamespace(L)
				.beginClass<StringHash>("StringHash")
				.addConstructor<void(*)(unsigned)>()
				.LUA_REGFUNC(StringHash, Value)
				.endClass();

			// Msg proxy
			getGlobalNamespace(L)
				.beginClass<MsgLua>("Msg")
				.addProperty("action", &MsgLua::GetAction)
				.addProperty("behaviorId", &MsgLua::GetBehaviorId)
				.addProperty("sourceObj", &MsgLua::GetSourceObj)
				.addProperty("subAction", &MsgLua::GetSubAction)
				.endClass();

			// Behavior proxy
			getGlobalNamespace(L)
				.beginClass<BehaviorLua>("Behavior")
				.addConstructor<void(*)(void)>()
				.addFunction("RegisterListening", &BehaviorLua::RegisterListeningLua)
				.addCFunction("Update", &BehaviorLua::UpdateLua)
				.addFunction("Init", &BehaviorLua::Init)
				.endClass();

			// Node proxy
			getGlobalNamespace(L)
				.beginClass<NodeLua>("Node")
				.addProperty("id", &NodeLua::GetId)
				.addProperty("parent", &NodeLua::GetParent, &NodeLua::SetParent)
				.addProperty("tag", &NodeLua::GetTag)
				.addProperty("transform", &NodeLua::GetTransform, &NodeLua::SetTransform)
				.addFunction("AddBehavior", &NodeLua::AddBehavior)
				.addFunction("AddChild", &NodeLua::AddChild)
				.addFunction("Draw", &NodeLua::Draw)
				.addFunction("HasAttr", &NodeLua::HasAttr)
				.addFunction("RemoveAttr", &NodeLua::RemoveAttr)
				.addFunction("RemoveBehavior", &NodeLua::RemoveBehavior)
				.addFunction("RemoveChild", &NodeLua::RemoveChild)
				.addFunction("SubmitChanges", &NodeLua::SubmitChanges)
				.addFunction("Update", &NodeLua::Update)
				.addFunction("UpdateTransform", &NodeLua::UpdateTransform)
				.endClass();

			getGlobalNamespace(L)
				.beginClass<LuaScripting>("Lua")
				.addStaticCFunction("RegisterBehavior", &LuaScripting::RegisterBehavior)
				.endClass();

#define lua_sample " \
mojo = { \
 attr1 = 12, \
 Update = function(delta, absolute) \
 end, \
 Init = function() \
 end \
 } \
 Lua.RegisterBehavior(mojo) "


			int status = luaL_loadstring(L, lua_sample);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
			}
			else {
				// execute program
				status = lua_pcall(L, 0, LUA_MULTRET, 0);
				if (status != 0) {
					std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
				}
			}

		}
		
		virtual void Update(const uint64 delta, const uint64 absolute) {
			
		}
	};

}// namespace