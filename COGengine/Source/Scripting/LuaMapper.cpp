#include "LuaMapper.h"
#include "ofUtils.h"
#include "Mesh.h"
#include <LuaBridge.h>
#include "Vec2i.h"
#include "Transform.h"
#include "StrId.h"
#include "Flags.h"
#include "Resources.h"
#include "MsgLua.h"
#include "BehaviorLua.h"
#include "NodeLua.h"
#include "SceneLua.h"
#include "FacadeLua.h"
#include "ResourcesLua.h"
#include "Settings.h""
#include "ComponentLua.h"
#include "StageLua.h"

using namespace luabridge;


namespace Cog {

	void LuaMapper::InitLuaMapping(lua_State* L) {

		// ofVec2f
		luabridge::getGlobalNamespace(L)
			.beginClass<ofVec2f>("ofVec2f")
			.addConstructor<void(*)(float, float)>()
			.addFunction(LUA_OPERATOR_PLUS, static_cast<ofVec2f(ofVec2f::*)(const ofVec2f &)const>(&ofVec2f::operator+))
			.addFunction(LUA_OPERATOR_MULT, static_cast<ofVec2f(ofVec2f::*)(const ofVec2f &)const>(&ofVec2f::operator*))
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(ofVec2f::*)(const ofVec2f &)const>(&ofVec2f::operator==))
			.addData("x", &ofVec2f::x)
			.addData("y", &ofVec2f::y)
			.endClass();

		// ofVec3f
		luabridge::getGlobalNamespace(L)
			.beginClass<ofVec3f>("ofVec3f")
			.addConstructor<void(*)(float, float, float)>()
			.addFunction(LUA_OPERATOR_PLUS, static_cast<ofVec3f(ofVec3f::*)(const ofVec3f &)const>(&ofVec3f::operator+))
			.addFunction(LUA_OPERATOR_MULT, static_cast<ofVec3f(ofVec3f::*)(const ofVec3f &)const>(&ofVec3f::operator*))
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(ofVec3f::*)(const ofVec3f &)const>(&ofVec3f::operator==))
			.addData("x", &ofVec3f::x)
			.addData("y", &ofVec3f::y)
			.addData("z", &ofVec3f::z)
			.endClass();

		// Text mesh
		luabridge::getGlobalNamespace(L)
			.beginClass<Text>("Text")
			.addFunction("AppendLine", &Text::AppendLine)
			.addFunction("AppendText", &Text::AppendText)
			.addFunction("GetHeight", &Text::GetHeight)
			.addFunction("GetText", &Text::GetText)
			.addFunction("GetTextHeight", &Text::GetTextHeight)
			.addFunction("GetTextWidth", &Text::GetTextWidth)
			.addFunction("GetWidth", &Text::GetWidth)
			.addFunction("SetText", &Text::SetText)
			.endClass();

		// Image mesh
		luabridge::getGlobalNamespace(L)
			.beginClass<Image>("Image")
			.addFunction("GetHeight", &Image::GetHeight)
			.addFunction("GetWidth", &Image::GetWidth)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<ofImage>("ofImage")
			.addFunction("getWidth", &ofImage::getWidth)
			.addFunction("getHeigt", &ofImage::getHeight)
			.endClass();

		// Vec2i
		luabridge::getGlobalNamespace(L)
			.beginClass<Vec2i>("Vec2i")
			.addConstructor<void(*)(int, int)>()
			.addData("x", &Vec2i::x)
			.addData("y", &Vec2i::y)
			.addFunction(LUA_OPERATOR_PLUS, static_cast<Vec2i(Vec2i::*)(const Vec2i &)const>(&Vec2i::operator+))
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(Vec2i::*)(const Vec2i &)const>(&Vec2i::operator==))
			.endClass();

		// Trans
		luabridge::getGlobalNamespace(L)
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
			.LUA_REGFUNC(Trans, CalcAngle)
			.LUA_REGFUNC(Trans, SetRotationAsAngleToPosition)
			.endClass();

		// StrId
		luabridge::getGlobalNamespace(L)
			.beginClass<StrId>("StrId")
			.addConstructor<void(*)(string)>()
			.LUA_REGFUNC(StrId, GetValue)
			.endClass();

		// Flags proxy
		luabridge::getGlobalNamespace(L)
			.beginClass<Flags>("Flags")
			.addConstructor<void(*)(StrId)>()
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(Vec2i::*)(const Vec2i &)const>(&Vec2i::operator==))
			.addFunction("HasState", &Flags::HasState)
			.addFunction("SetState", &Flags::SetState)
			.addFunction("SwitchState", &Flags::SwitchState)
			.addFunction("ResetState", &Flags::ResetState)
			.endClass();

		// Settings proxy
		luabridge::getGlobalNamespace(L)
			.beginClass<Settings>("Settings")
			.addConstructor<void(*)()>()
			.addFunction("GetSettingVal", static_cast<string (Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValInt", &Settings::GetSettingValInt)
			.addFunction("GetSettingValFloat", &Settings::GetSettingValFloat)
			.addFunction("GetSettingValBool", &Settings::GetSettingValBool)
			.addFunction("GetSetting", &Settings::GetSetting)
			.addFunction("SetSetting", &Settings::SetSetting)
			.endClass();


		luabridge::getGlobalNamespace(L)
			.beginClass<Setting>("Setting")
			.addConstructor<void(*)(string)>()
			.addFunction("GetItem", &Setting::GetItem)
			.addFunction("GetItemVal", &Setting::GetItemVal)
			.addFunction("GetItemValDouble", &Setting::GetItemValDouble)
			.addFunction("GetItemValInt", &Setting::GetItemValInt)
			.addFunction("GetItemValFloat", &Setting::GetItemValFloat)
			.addFunction("GetItemValBool", &Setting::GetItemValBool)
			.addFunction("GetItemVals", &Setting::GetItemVals)
			.addFunction("AddItem", &Setting::AddItem)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<SettingItem>("SettingItem")
			.addConstructor<void(*)(string)>()
			.addFunction("AddValues", static_cast<string(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("HasMoreValues", &SettingItem::HasMoreValues)
			.addFunction("GetValues", &SettingItem::GetValues)
			.addFunction("GetValStr", &SettingItem::GetValStr)
			.addFunction("GetValDouble", &SettingItem::GetValDouble)
			.addFunction("GetValInt", &SettingItem::GetValInt)
			.addFunction("GetValFloat", &SettingItem::GetValFloat)
			.addFunction("GetValBool", &SettingItem::GetValBool)
			.endClass();

		BehaviorLua::InitLuaMapping(L);
		MsgLua::InitLuaMapping(L);
		NodeLua::InitLuaMapping(L);
		SceneLua::InitLuaMapping(L);
		FacadeLua::InitLuaMapping(L);
		ComponentLua::InitLuaMapping(L);
		ResourcesLua::InitLuaMapping(L);
		StageLua::InitLuaMapping(L);
	}

} // namespace