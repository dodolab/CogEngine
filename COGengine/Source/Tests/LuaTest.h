#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;


#include <LuaBridge.h>
#include <iostream>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

struct BindObject {
public:

	BindObject() {
		staticInt = 0;
		staticFloat = 0;
	}

	
	static int staticInt;
	static float staticFloat;

	int intProp;
	float floatProp;

	static int Add(int x, int y) {
		return x + y;
	}

	int GetIntProp() const {
		return intProp;
	}

	void SetIntProp(int newProp) {
		this->intProp = newProp;
	}

};

int BindObject::staticInt;
float BindObject::staticFloat;

using namespace luabridge;

int lastGlobalFuncParam = 0;
int LuaGlobalFunction(lua_State *L)
{
	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n) {
		lastGlobalFuncParam = lua_tointeger(L, n);
	}

	lua_pushnumber(L, 123); // return value
	return 1; // number of return values
}


TEST_CASE("Lua tests", "[class]")
{

	SECTION("Simple lua Lua-C++ function call")
	{
		// create lua state
		lua_State* pl = luaL_newstate();
		// enable access to standard libs
		luaL_openlibs(pl);


		lua_register(pl, "LuaGlobalFunction", LuaGlobalFunction);

#define lua_sample "\
param = 1234 \
param = LuaGlobalFunction(param)"

		// reset stack index
		lua_settop(pl, 0);

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}
		lua_getglobal(pl, "param");
		// stack index starts with 1
		int param = lua_tointeger(pl, 1);
		// remove the value from the stack
		
		REQUIRE(param == 123);
		REQUIRE(lastGlobalFuncParam == 1234);

		lua_pop(pl, 1);
		lua_close(pl);
	}

	SECTION("Simple lua C++-Lua function call")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

#define lua_sample " \
		function add(a, b) \
			return (a + b); \
		end; "
		
		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}

		lua_getglobal(pl, "add");
		lua_pushnumber(pl, 10);
		lua_pushnumber(pl, 20);

		// 2 input parameters and 1 return parameter
		lua_call(pl, 2, 1);

		int result = (int)lua_tonumber(pl, -1);

		REQUIRE(result == 30);

		lua_pop(pl, 1);
		lua_close(pl);
	}

	SECTION("Lua classes")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

#define lua_sample " \
function Person(salary) \
 local person = { \
  salary = salary; \
  GetSalary = function() \
  return salary; \
  end; \
  SetSalary = function(howMuch) \
  salary = howMuch; \
  end; \
  } \
  return person; \
end \
thomas = Person(12000)  "

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}

		LuaRef s = getGlobal(pl, "thomas");
		LuaRef getSalary = s["GetSalary"];
		LuaRef setSalary = s["SetSalary"];

		int result1 = getSalary();
		setSalary(13000);
		int result2 = getSalary();
		
		REQUIRE(result1 == 12000);
		REQUIRE(result2 == 13000);

		//don't call lua_close; LuaBridge closes it automatically
	}

	SECTION("LuaBridge arrays")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

#define lua_sample " \
ghost = { \
  name = \"LittleGhost\", \
  sprite = \"ghost_sprite.png\", \
  size = 12 \
}"

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}

		LuaRef s = getGlobal(pl, "ghost");
		LuaRef nameRef = s["name"];
		LuaRef spriteRef = s["sprite"];
		LuaRef sizeRef = s["size"];

		string name = nameRef.cast<string>();
		string sprite = spriteRef.cast<string>();
		int size = sizeRef.cast<int>();

		REQUIRE(name.compare("LittleGhost") == 0);
		REQUIRE(sprite.compare("ghost_sprite.png") == 0);
		REQUIRE(size == 12);

		//don't call lua_close; LuaBridge closes it automatically
	}

	SECTION("LuaBridge C++-to-Lua")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

		getGlobalNamespace(pl)
			.beginClass<BindObject>("BindObject")
			.addConstructor <void(*) (void)>() // necessary
			.addStaticFunction("Add", &BindObject::Add)
			.addStaticData("staticInt", &BindObject::staticInt)
			.addStaticData("staticFloat", &BindObject::staticFloat)
			.addData("floatProp", &BindObject::floatProp)
			.addProperty("intProp", &BindObject::GetIntProp, &BindObject::SetIntProp)
			.endClass();

#define lua_sample " \
 bind = BindObject() \
 bind.floatProp = 85 \
 bind.intProp = BindObject.Add(1,4) \
 BindObject.staticInt = 120 \
 BindObject.staticFloat = 7 "

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			status = lua_pcall(pl, 0, LUA_MULTRET, 0);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
			}
		}

		LuaRef s = getGlobal(pl, "bind");
		auto bindObj = s.cast<BindObject>();
		REQUIRE(BindObject::staticFloat == 7);
		REQUIRE(BindObject::staticInt == 120);
		REQUIRE(bindObj.floatProp == 85);
		REQUIRE(bindObj.intProp == 5);
	}
}