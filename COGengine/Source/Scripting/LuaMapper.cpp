#include "LuaMapper.h"
#include "ofUtils.h"
#include "Mesh.h"
#include <LuaBridge.h>
#include "Vec2i.h"
#include "Transform.h"
#include "StrId.h"
#include "Flags.h"

using namespace luabridge;


namespace Cog {

	void LuaMapper::InitMapping(lua_State* L) {

		// ofVec2f
		luabridge::getGlobalNamespace(L)
			.beginClass<ofVec2f>("ofVec2f")
			.addConstructor<void(*)(float, float)>()
			.addData("x", &ofVec2f::x)
			.addData("y", &ofVec2f::y)
			.endClass();

		// ofVec3f
		luabridge::getGlobalNamespace(L)
			.beginClass<ofVec3f>("ofVec3f")
			.addConstructor<void(*)(float, float, float)>()
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
			.addFunction("HasState", &Flags::HasState)
			.addFunction("SetState", &Flags::SetState)
			.addFunction("SwitchState", &Flags::SwitchState)
			.addFunction("ResetState", &Flags::ResetState)
			.endClass();

	}

} // namespace