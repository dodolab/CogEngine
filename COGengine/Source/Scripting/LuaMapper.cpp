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
#include "TransformMath.h"
#include "NodeBuilderLua.h"

using namespace luabridge;


namespace Cog {

	void LuaMapper::InitLuaMapping(lua_State* L) {

		// static functions
		luabridge::getGlobalNamespace(L)
			.addFunction("ofRandom", static_cast<float(*)(float, float)>(&ofRandom))
			.addFunction("floor", static_cast<double(*)(double)>(&floor));

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

		// TransformEnt
		luabridge::getGlobalNamespace(L)
			.beginClass<TransformEnt>("TransformEnt")
			.addConstructor <void(*) (ofVec2f, stenum, float)>()
			.LUA_REGDATA(TransformEnt, anchor)
			.LUA_REGDATA(TransformEnt, name)
			.LUA_REGDATA(TransformEnt, pos)
			.LUA_REGDATA(TransformEnt, pType)
			.LUA_REGDATA(TransformEnt, rotation)
			.LUA_REGDATA(TransformEnt, rotationCentroid)
			.LUA_REGDATA(TransformEnt, size)
			.LUA_REGDATA(TransformEnt, sType)
			.LUA_REGDATA(TransformEnt, zIndex)
			.endClass();

		// TransformMath
		luabridge::getGlobalNamespace(L)
			.beginClass<TransformMath>("TransformMath")
			.addConstructor <void(*) (void)>()
			.addFunction("SetTransform", static_cast<void(TransformMath::*)(Node*, Node*, TransformEnt&, int, int)>(&TransformMath::SetTransform))
			.addFunction("SetTransform", reinterpret_cast<void(TransformMath::*)(NodeLua*, NodeLua*, TransformEnt&, int, int)>(&TransformMath::SetTransform))
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
			.addFunction("GetSettingVal", static_cast<string(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValInt", static_cast<int(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValFloat", static_cast<float(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValBool", static_cast<bool(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSetting", &Settings::GetSetting)
			.addFunction("SetSetting", &Settings::SetSetting)
			.endClass();

		// Setting
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
		NodeBuilderLua::InitLuaMapping(L);
	}

} // namespace