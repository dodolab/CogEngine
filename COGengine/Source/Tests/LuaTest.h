#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


int lastGlobalFuncParam = 0;
int LuaGlobalFunction(lua_State *L)
{
	int argc = lua_gettop(L);

	std::cerr << "-- my_function() called with " << argc
		<< " arguments:" << std::endl;

	for (int n = 1; n <= argc; ++n) {
		lastGlobalFuncParam = lua_tointeger(L, n);
	}

	lua_pushnumber(L, 123); // return value
	return 1; // number of return values
}


TEST_CASE("Lua tests", "[class]")
{

	SECTION("Simple lua function call")
	{
		// create lua state
		lua_State* pl = lua_open();
		// enable access to standard libs
		luaopen_base(pl);
		luaopen_table(pl);
		luaopen_string(pl);
		luaopen_math(pl);


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
}